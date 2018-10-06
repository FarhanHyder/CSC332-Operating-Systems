/*[cpu emulator]
author: Farhan Hyder
CSC 332 - Assignment 1
Date: 10/04/2018
*/

#include <sys/socket.h> 	//socket connection 
#include <string.h> 		//strcmp, memset
#include <unistd.h>			//read, close
#include <arpa/inet.h>		//struct sockaddr_in, htons,inet_pton
#include <netinet/in.h> 	//dont know what it does exactly, but w/out this sometimes the connection fails
#include <iostream>			//stoi,cout,perror

#define PORT 8080
using namespace std;

struct PCB{
		char name[30];
		int PID;
		char status[30];
		int priority;
		int interruptable;
		int est_runtime;
		int est_remaining_runtime;
};

int setupTCPClient(int port);
   
int main(int argc, char const *argv[]) 
{   
	int socket = 0, port = PORT; 
	PCB* ready_queue = new PCB();
	if(argc>1)
		port = stoi(argv[0]);	//string to int -> passed from init

	// setup TCP connection for client
	socket = setupTCPClient(port);
	if(socket<0)
		return -1;

	int cyclesPerBurst=200;

	// ready for sending and receiving data
	// keep running until scheduler sends in processes with name stop
	while(true ){
		read( socket , ready_queue, sizeof(PCB));
		if(strcmp(ready_queue->name, "stop")==0)
			break;
		cout<<"CPU: Exec "<<ready_queue->name<<","<<ready_queue->PID<<","<<ready_queue->status<<","
			<<ready_queue->priority<<","<<ready_queue->interruptable<<","<<ready_queue->est_runtime
			<<","<<ready_queue->est_remaining_runtime<<endl;

		ready_queue->est_remaining_runtime-=cyclesPerBurst;
		send(socket , ready_queue , sizeof(PCB) , 0 );
	}	
	close(socket);
	return 0; 
}



int setupTCPClient(int port)
{
	struct sockaddr_in address; 
	int sock = 0; 
	struct sockaddr_in serv_addr; 

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		perror("\nERROR: socket creation failed!\n"); 
		return -1; 
	} 

	memset(&serv_addr, '0', sizeof(serv_addr)); 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(port); 

	// convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
	{ 
		perror("\nERROR: invalid address!\n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		perror("\nERROR : connection failed!\n"); 
		return -1; 
	} 
	return sock;

}
