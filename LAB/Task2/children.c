/*  task2 : part #1
    author: Farhan Hyder
    submission date: 10/05/2018
    description: 
		- the parent process produce two child processes. 
		- One prints out "I am child one, my pid is: " PID, 
		- and the other prints out "I am child two, my pid is: " PID. 
		- Guarantee that the parent terminates after the children terminate.
*/

#include <sys/types.h>	//getpid, fork, waitpid
#include <unistd.h>		//getpid, fork
#include<stdio.h>		//printf
#include <sys/wait.h>	//waitpid

int main()
{
	pid_t child1_PID, child2_PID;
	int *child1_status, *child2_status;

	child1_PID = fork();
	if (child1_PID == 0)
	//child one process
	{
		printf("I am child one, my PID is : %d\n",getpid());
	}
	else
	//back to parent process
	{
		child2_PID = fork();
		if (child2_PID == 0)
		//child two process
		{
			printf("I am child two, my PID is : %d\n",getpid());
		}
		else
		{
			waitpid(child1_PID,child1_status,0);	//wait for child#1
			waitpid(child2_PID,child2_status,0);	//wait for child#2
		}
	}	
	return 0;
}
