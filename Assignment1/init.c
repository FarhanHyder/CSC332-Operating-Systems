/*[init]
author: Farhan Hyder
CSC 332 - Assignment 1
Date: 10/04/2018
*/


#include <stdio.h>		//perror
#include <stdlib.h>		//exit
#include <sys/wait.h>	//wait
#include<unistd.h>		//execvp


int main()
{
	pid_t cpu_PID, scheduler_PID;
	//send the port # through the command line argument, on which both processes will communicate on- 
	char *args[]={"4545",NULL};

	cpu_PID=fork();
	if(cpu_PID==0){
		// inside cpu emulator fork
			// start CPU
		execvp("./cpu_emulator",args);
			// end CPU
		exit(0);
	}
	else if(cpu_PID>0){
		// inside parent fork
		scheduler_PID=fork();
		if(scheduler_PID==0){
			// inside scheduler fork
				//	start scheduler
			execvp("./scheduler",args);
				// end scheduler
			exit(0);

		}
		else if(scheduler_PID>0){
			// inside parent
			wait(NULL);	// wait for both child to die :(
		}
		else{
			perror("ERROR :  scheduler fork() creation failed!\n");
		}
	}
	else{
		perror("ERROR :  CPU emulator fork() creation failed!\n");
	}
	return 0;
}


