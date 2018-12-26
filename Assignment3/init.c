/*[init]
	author: Farhan Hyder
	CSC 332 - Assignment 3
	Date: 12/16/2018
*/


#include <stdio.h>		//perror
#include <stdlib.h>		//exit
#include <sys/wait.h>	//wait
#include <unistd.h>		//execvp
#include <string.h>

int main(int argc, char const *argv[])
{
	char schedulingPolicy[30], rrScheduler[30], time_quantum[20];
	pid_t cpu_PID, scheduler_PID;

	if (argc < 2)
	{
		printf("ERROR :  You must give scheduling policy as an argument\n");
		return 0;
	}

	if (strcmp(argv[1], "RR") == 0)
	{
		if (argc < 4){
			printf("ERROR :  Three arguments are expected (RR, scheduling policy, time quantum)\n");
			return 0;
		}
	}

	//send the port # through the command line argument, on which both processes will communicate on- 
	strcpy(schedulingPolicy, argv[1]);

	if (argc == 2)
	{
		strcpy(rrScheduler, " ");
		strcpy(time_quantum, "1");
	}
	else
	{
		strcpy(rrScheduler, argv[2]);
		strcpy(time_quantum, argv[3]);
	}

	char *args[]={"4545",schedulingPolicy,rrScheduler,time_quantum,NULL};

	cpu_PID=fork();
	if(cpu_PID==0)
	{
		// inside cpu emulator fork
			// start CPU
		execvp("./cpu_emulator",args);
			// end CPU
		exit(0);
	}
	else if(cpu_PID>0){
		// inside parent fork
		scheduler_PID=fork();
		if(scheduler_PID==0)
		{
			// inside scheduler fork
				//	start scheduler
			execvp("./scheduler",args);
				// end scheduler
			exit(0);

		}
		else if(scheduler_PID>0)
		{
			// inside parent
			wait(NULL);	// wait for both child to die :(
		}
		else
			perror("ERROR :  scheduler fork() creation failed!\n");
	}
	else
		perror("ERROR :  CPU emulator fork() creation failed!\n");

	return 0;
}


