/*[scheduler]
	author: Farhan Hyder
	CSC 332 - Assignment 3
	Date: 12/16/2018
*/

#include <string.h> 		//strcpy
#include <unistd.h>			//read, close
#include <arpa/inet.h>		//struct sockaddr_in, htons,inet_pton, and other socket stuffs
#include <netinet/in.h> 	//dont know what it does exactly, but w/out this sometimes the connection fails
#include <vector>			//vector, although it works w/out importing it for some reason
#include <fstream>			//ifstream
#include <queue>			//queue stuffs
#include <iostream>			//cout
#include <sstream>
#include <algorithm>
#include "headers.h"

#define PORT 8080
using namespace std;

int setupTCPServer(int port);
vector<PCB> read_processes_from_text_file(string fileName);
PCB dispatchToCPU(PCB process, int socket);
void FCFS(vector<PCB> processes,int socket);
void SJN(vector<PCB> processes,int socket);
void roundRobin(vector<PCB> processes,int socket,int time_quantum,char schedule[20]);
void stopCPU(int socket);
void processCompleted(PCB process);
void Priority(vector<PCB> processes,int socket);
void updateProcessQueue(queue<PCB> *processQueue, queue<PCB> *waitQueue, int TIME);
void statistics(string method, vector<PCB> completedProcesses);
PCB getShortestJob(queue<PCB> *processQueue);
PCB getShortestRemainingTimeNext(queue<PCB> *processQueue);

int main(int argc, char const *argv[]) 
{ 
	int socket = 0, port = PORT, time_quantum; 
	char schedulingPolicy[30], rrScheduler[30];

	PCB* buffer = new PCB();
	port = stoi(argv[0]);

	strcpy(schedulingPolicy, argv[1]);

	if(argc > 2)
	{
		strcpy(rrScheduler, argv[2]);
		time_quantum = stoi(argv[3]);
	}
	
	socket=setupTCPServer(port);

	//Read all processes from processes.txt
	vector<PCB>  process=read_processes_from_text_file("processes_3.txt") ; 
		
	//Ready for sending and receiving data	

	if(strcmp(schedulingPolicy, "RR") == 0)
		roundRobin(process,socket,time_quantum,rrScheduler);

	else if(strcmp(schedulingPolicy, "Priority") == 0)
		Priority(process,socket);

	else if(strcmp(schedulingPolicy, "FCFS") == 0)
		FCFS(process,socket);

	else if(strcmp(schedulingPolicy, "SJN") == 0)
		SJN(process,socket);


	stopCPU(socket); //time to stop CPU
	close(socket);
	return 0; 
}

void updateProcessQueue(queue<PCB> *processQueue, queue<PCB> *waitQueue, int TIME)
{
	if (waitQueue->size() == 0)
		return;

	PCB temp = waitQueue->front();

	if (temp.arrival_time <= TIME)
	{
		waitQueue->pop();
		processQueue->push(temp);
		updateProcessQueue(processQueue, waitQueue, TIME);
	}
}

PCB getShortestJob(queue<PCB> *processQueue)
{
	int shortest_run_time = -1;
	int shortest_index = 0;
	int N = processQueue->size();
	PCB temp;
	vector<PCB> checked;

	for (int i = 0; i < N; i++)
	{
		temp = processQueue->front();
		processQueue->pop();
		if (shortest_run_time == -1 || temp.est_runtime < shortest_run_time)
		{
			shortest_run_time = temp.est_runtime;
			shortest_index = i;
		}
		checked.push_back(temp);
	}

	for (int i = 0; i < checked.size(); i++)
	{
		if (i == shortest_index)
			temp = checked[i];

		else
			processQueue->push(checked[i]);
	}

	return temp;
}

PCB getShortestRemainingTimeNext(queue<PCB> *processQueue)
{
	int shortest_run_time = -1;
	int shortest_index = 0;
	int N = processQueue->size();
	PCB temp;
	vector<PCB> checked;

	for (int i = 0; i < N; i++)
	{
		temp = processQueue->front();
		processQueue->pop();
		if (shortest_run_time == -1 || temp.est_remaining_runtime < shortest_run_time)
		{
			shortest_run_time = temp.est_remaining_runtime;
			shortest_index = i;
		}
		checked.push_back(temp);
	}

	for (int i = 0; i < checked.size(); i++)
	{
		if (i == shortest_index)
			temp = checked[i];

		else
			processQueue->push(checked[i]);
	}

	return temp;
}

PCB getLowestPriorityJob(queue<PCB> *processQueue)
{
	int lowest_priority = -1;
	int lowest_priority_index = 0;
	int N = processQueue->size();
	PCB temp;
	vector<PCB> checked;

	for (int i = 0; i < N; i++){
		temp = processQueue->front();
		processQueue->pop();
		if (lowest_priority == -1 || temp.priority < lowest_priority)
		{
			lowest_priority = temp.priority;
			lowest_priority_index = i;
		}
		checked.push_back(temp);
	}

	for (int i = 0; i < checked.size(); i++)
	{
		if (i == lowest_priority_index)
			temp = checked[i];

		else
			processQueue->push(checked[i]);
	}

	return temp;
}

void statistics(string method, vector<PCB> completedProcesses)
{
	cout << "\n###### [" << method << " Schedule] Statistics ######\n";

	float average_turnaround_time = 0;
	float average_wait_time = 0;
	float longest_wait_time = -1;
	float total_run_time = 0;
	int total_number_of_context_switches = 0;

	for (int i = 0; i < completedProcesses.size(); i++)
	{
		average_turnaround_time += completedProcesses[i].turnaround_time;
		average_wait_time += completedProcesses[i].wait_time;

		if (completedProcesses[i].wait_time > longest_wait_time)
			longest_wait_time = completedProcesses[i].wait_time;

		total_run_time += completedProcesses[i].est_runtime;
		total_number_of_context_switches += completedProcesses[i].context_switches;
	}

	average_turnaround_time /= completedProcesses.size();
	average_wait_time /= completedProcesses.size();
	total_run_time += (total_number_of_context_switches*2);
	cout << "Average turnaround time: " << average_turnaround_time << endl;
	cout << "Average wait time: " << average_wait_time << endl;
	cout << "Longest wait time: " << longest_wait_time << endl;
	cout << "Total run time: " << total_run_time << endl;
	cout << "Total number of context_switches: " << total_number_of_context_switches << endl;
	cout << "===================================================" << endl;
}

void FCFS(vector<PCB> processes,int socket)
{
	cout<<"###### [First Come First Served Schedule] Start ######\n";
	cout << "Process/Turnaround time/Waiting Time/Context Switches/State" << endl;

	int TIME = 0, completionTime = 0;
	int processes_completed = 0;
	queue<PCB> processQueue, waitQueue;
	vector<PCB> completedProcesses;

	for (const auto& e: processes)
  		waitQueue.push(e);

  	while (processes_completed < processes.size())
	{
  		if (waitQueue.size() > 0) updateProcessQueue(&processQueue, &waitQueue, TIME);
  		if (processQueue.size() == 0)
		{
  			TIME++;
  			continue;
  		}

  		TIME += 2; //context switch time
  		PCB temp = processQueue.front();
		processQueue.pop();

		while (temp.est_remaining_runtime > 0)
		{
			temp=dispatchToCPU(temp,socket);
			TIME++;
			if (waitQueue.size() > 0) updateProcessQueue(&processQueue, &waitQueue, TIME);
		}

		completionTime = TIME;
		temp.turnaround_time = completionTime - temp.arrival_time;
		temp.wait_time = temp.turnaround_time - temp.est_runtime;
		processCompleted(temp);
		completedProcesses.push_back(temp);
		processes_completed += 1;
  		TIME++;
  	}
  	statistics("First Come First Served", completedProcesses);
}

void SJN(vector<PCB> processes,int socket)
{
	cout<<"###### [Shortest Job Next Schedule] Start ######\n";
	cout << "Process/Turnaround time/Waiting Time/Context Switches/State" << endl;

	int TIME = 0, completionTime = 0;
	int processes_completed = 0;
	queue<PCB> processQueue, waitQueue;
	vector<PCB> completedProcesses;

	for (const auto& e: processes)
  		waitQueue.push(e);

	while (processes_completed < processes.size())
	{
  		if (waitQueue.size() > 0) updateProcessQueue(&processQueue, &waitQueue, TIME);
  		if (processQueue.size() == 0)
		{
  			TIME++;
  			continue;
  		}

  		TIME += 2; //context switch time
  		PCB temp = getShortestJob(&processQueue);

		while (temp.est_remaining_runtime > 0)
		{
			temp=dispatchToCPU(temp,socket);
			TIME++;
			if (waitQueue.size() > 0) updateProcessQueue(&processQueue, &waitQueue, TIME);
		}

		completionTime = TIME;
		temp.turnaround_time = completionTime - temp.arrival_time;
		temp.wait_time = temp.turnaround_time - temp.est_runtime;
		processCompleted(temp);
		completedProcesses.push_back(temp);
		processes_completed += 1;
  		TIME++;
  	}

  	statistics("Shortest Job Next", completedProcesses);
}


void Priority(vector<PCB> processes,int socket)
{
	cout<<"###### [Priority Schedule] Start ######\n";
	cout << "Process/Turnaround time/Waiting Time/Context Switches/State" << endl;

	int TIME = 0, completionTime = 0;
	int processes_completed = 0;
	queue<PCB> processQueue, waitQueue;
	vector<PCB> completedProcesses;

	for (const auto& e: processes)
  		waitQueue.push(e);

	while (processes_completed < processes.size())
	{
  		if (waitQueue.size() > 0) updateProcessQueue(&processQueue, &waitQueue, TIME);
  		if (processQueue.size() == 0)
		{
  			TIME++;
  			continue;
  		}

  		TIME += 2; //context switch time
  		PCB temp = getLowestPriorityJob(&processQueue);

		while (temp.est_remaining_runtime > 0)
		{
			temp=dispatchToCPU(temp,socket);
			TIME++;
			if (waitQueue.size() > 0) updateProcessQueue(&processQueue, &waitQueue, TIME);
		}

		completionTime = TIME;
		temp.turnaround_time = completionTime - temp.arrival_time;
		temp.wait_time = temp.turnaround_time - temp.est_runtime;
		processCompleted(temp);
		completedProcesses.push_back(temp);
		processes_completed += 1;
  		TIME++;
  	}

  	statistics("Priority", completedProcesses);
}

void roundRobin(vector<PCB> processes,int socket,int time_quantum,char schedule[20])
{
	cout<<"\n###### [Round-Robin] @" << time_quantum << " time quantum using " << schedule << " Start######\n";
	cout << "Process/Turnaround time/Waiting Time/Context Switches/State" << endl;

	int TIME = 0, completionTime = 0;
	int processes_completed = 0;
	queue<PCB> processQueue, waitQueue;
	vector<PCB> completedProcesses;

	for (const auto& e: processes)
  		waitQueue.push(e);

  	while (processes_completed < processes.size())
	{
  		if (waitQueue.size() > 0) updateProcessQueue(&processQueue, &waitQueue, TIME);
  		if (processQueue.size() == 0)
		{
  			TIME++;
  			continue;
  		}

  		TIME += 2; //context switch time
  		PCB temp;

  		if (strcmp(schedule, "FCFS") == 0)
		{
  			temp = processQueue.front();
			processQueue.pop();
	  	}
	  	else if (strcmp(schedule, "SRTN") == 0)
	  		temp = getShortestRemainingTimeNext(&processQueue);
	  	
	  	else if (strcmp(schedule, "Priority") == 0)
	  		temp = getLowestPriorityJob(&processQueue);
	  	

	  	int time_passed = time_quantum;

	  	if (temp.est_remaining_runtime < time_passed)
	  		time_passed = temp.est_remaining_runtime;
	  	

	  	temp=dispatchToCPU(temp,socket);

	  	while (time_passed > 0)
		{
			time_passed -= 1;
			TIME++;
			if (waitQueue.size() > 0) updateProcessQueue(&processQueue, &waitQueue, TIME);
		}

	  	if (temp.est_remaining_runtime > 0)
			processQueue.push(temp);
		
		else
		{
			completionTime = TIME;
			temp.turnaround_time = completionTime - temp.arrival_time;
			temp.wait_time = temp.turnaround_time - temp.est_runtime;
			processCompleted(temp);
			completedProcesses.push_back(temp);
			processes_completed += 1;
	  		TIME++;
		}	
  	}

  	statistics("Round Robin", completedProcesses);
}


void stopCPU(int socket)
	//sends "stop" to stop CPU
{
	PCB stop;
	strcpy(stop.program_name, "stop");
	send(socket , &stop , sizeof(PCB) , 0 );
}

PCB dispatchToCPU(PCB process, int socket)
	//dispatch data to CPU
{
	int valread;
	send(socket , &process , sizeof(PCB) , 0 );
	valread = read( socket , &process, sizeof(PCB));
	return process;
}

void processCompleted(PCB process)
	//process completion display
{
	cout << "Process: " <<process.PID<<", TAT: " << process.turnaround_time << ", WT: " << process.wait_time << ", #Context: " << process.context_switches  << ", " << process.program_name<<" Completed.\n";
}

vector<PCB> read_processes_from_text_file(string fileName)
	//read process list from text file
{
	int processCount=0;
	vector<PCB>  process;
	ifstream iFile(fileName);
	string line;
	char token[30];
	getline(iFile, line);

	while (getline(iFile, line))
	{
		process.push_back(PCB());
		replace(line.begin(), line.end(), ',', ' ');
	    stringstream ss(line);
	    ss >> process[processCount].arrival_time;
	    ss >> token;
	    strcpy(process[processCount].program_name, token);
	    ss >> token;
	    process[processCount].PID = stoi(token);
	    ss >> token;
	    ss >> token;
	    process[processCount].priority = stoi(token);
	    ss >> token;
	    process[processCount].interruptable = stoi(token);
	    ss >> token;
	    process[processCount].est_runtime = stoi(token);
	   	ss >> token;
	    process[processCount].est_remaining_runtime = stoi(token);
	    process[processCount].wait_time = 0;
		process[processCount].context_switches = 0;
	    processCount++;
	}

	cout<< "Scheduler:: Got "<<processCount<<" processes from file\n";
	return process;
}

int setupTCPServer(int port)
	//sets up TCP server with given port
{
	int server_fd, new_socket; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 

	// creates socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 

	
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( port ); 

	// attach socket to the port  
	if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) 
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	}
	return new_socket;
}
