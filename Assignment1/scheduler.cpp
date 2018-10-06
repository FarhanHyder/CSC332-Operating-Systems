/*[scheduler]
author: Farhan Hyder
CSC 332 - Assignment 1
Date: 10/04/2018
*/

#include <string.h> 		//strcpy
#include <unistd.h>			//read, close
#include <arpa/inet.h>		//struct sockaddr_in, htons,inet_pton, and other socket stuffs
#include <netinet/in.h> 	//dont know what it does exactly, but w/out this sometimes the connection fails
#include <vector>			//vector, although it works w/out importing it for some reason
#include <fstream>			//ifstream
#include <queue>			//queue stuffs
#include <iostream>			//cout

#define PORT 8080
using namespace std;

struct PCB
{
		char program_name[30];
		int PID;
		char status[30];
		int priority;
		int interruptable;
		int est_runtime;
		int est_remaining_runtime;
};


int setupTCPServer(int port);
vector<PCB> read_processes_from_text_file(string fileName);
PCB dispatchToCPU(PCB process, int socket);
void roundRobin(vector<PCB> processes,int socket);
void stopCPU(int socket);
void processCompleted(PCB process);
void PriorityFirst(vector<PCB> processes,int socket);

int main(int argc, char const *argv[]) 
{ 
	int socket = 0, port = PORT; 

	PCB* buffer = new PCB();

	if(argc>1)
		port = stoi(argv[0]);	//string to int -> passed from init

	socket=setupTCPServer(port);

	//Read all processes from processes.txt
	vector<PCB>  process=read_processes_from_text_file("processes.txt") ; 
	
	
	
	//Ready for sending and receiving data
	string schedulingPolicy="TestMode";
	

	if(schedulingPolicy=="TestMode"){
		roundRobin(process,socket);
		PriorityFirst(process,socket);
	}		
	else if(schedulingPolicy=="roundrobin")
		roundRobin(process,socket);
        else if(schedulingPolicy=="priority")
                PriorityFirst(process,socket);

	stopCPU(socket);	//time to stop CPU

	close(socket);
	return 0; 
}

void roundRobin(vector<PCB> processes,int socket)
{
	cout<<"\n###### [Round-Robin] @200 cycles/process Start######\n\n";
	queue<PCB> processQueue;
	for (const auto& e: processes)
  		processQueue.push(e);


	while(!processQueue.empty()){
		PCB temp = processQueue.front();
		processQueue.pop();
		temp=dispatchToCPU(temp,socket);
		if(temp.est_remaining_runtime<=0)
			processCompleted(temp);
		else{
			processQueue.push(temp);
		}

	}
	
	cout<<"\n###### Round-Robin End######\n\n\n";
}

void PriorityFirst(vector<PCB> processes,int socket)
{
	cout<<"###### [Priority First Schedule] Start ######\n\n";
	priority_queue<pair<int,int> > processQueue;
	vector<pair<int,int> > holdout;
	int currPriority=0;
	for (int i=0;i< processes.size();i++)
  		processQueue.push(make_pair(-processes[i].priority,-i));

	while(!processQueue.empty() || holdout.size()>0){
		
		if(processQueue.empty()){
			for (int i=0;i< holdout.size();i++)
  				processQueue.push(holdout[i]);
			holdout.resize(0);
		}

		pair<int,int> temp=processQueue.top();
		if(currPriority< -temp.first && holdout.size()>0){// bring back process with currpriority from holdout
			for (int i=0;i< holdout.size();i++)
  				processQueue.push(holdout[i]);
			holdout.resize(0);
			temp=processQueue.top();
			
		}
		else{// Move to next priority
			currPriority=-temp.first;
		}
			
		processQueue.pop();
		processes[-temp.second]=dispatchToCPU(processes[-temp.second],socket);
		if(processes[-temp.second].est_remaining_runtime<=0)
			processCompleted(processes[-temp.second]);
		else{
		
			holdout.push_back(temp);
		}

	}
	
	cout<<"\n###### [Priority First Schedule] End######\n";
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
	cout<<"\tScheduler:  Process "<<process.PID<<", " <<process.program_name<<" Completed.\n";
}

vector<PCB> read_processes_from_text_file(string fileName)
	//read process list from text file
{
	int processCount=0;
	vector<PCB>  process;
	ifstream iFile(fileName);
	if(!iFile.is_open())
		cout<<"Can't open "<<fileName<<endl;
	while (!iFile.eof())
	{
		process.push_back(PCB());
		string temp;
	
		getline(iFile, temp, ',');
		strcpy(process[processCount].program_name, temp.c_str());  //name
		iFile>>process[processCount].PID;	//PID
		iFile.ignore();// ignore comma
		getline(iFile, temp, ',');
		strcpy(process[processCount].status, temp.c_str());	//status

		iFile>>process[processCount].priority;
		iFile.ignore();// ignore comma
		iFile>>process[processCount].interruptable;
		iFile.ignore();// ignore comma
		iFile>>process[processCount].est_runtime;
		iFile.ignore();// ignore comma
		iFile>>process[processCount].est_remaining_runtime;

		processCount++;
		while(iFile.peek()=='\n' || iFile.peek()==' ')
			iFile.ignore();
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
