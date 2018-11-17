#include <stdio.h>
#include <stdlib.h>
#include "sem2.h"

#include <unistd.h>		//fork, getpid
#include <sys/types.h>	//wait, fork, getpid
#include <sys/wait.h>	//wait


#define CHILD      			0  			/* Return value of child proc from fork call */
#define TRUE       			0  
#define FALSE      			1
#define PERMS			    0666 		/* 0666 - To grant read and write permissions */

FILE *fp1, *fp2, *fp3, *fp4, *fp5, *fp6, *fp7;			/* File Pointers */
int sem;								/* Semaphore */

int main()
{
	int pid;						// Process ID after fork call
	int i;							// Loop index
	int N;							// Number of times dad does update
	int N_Att;						// Number of time sons allowed to do update
	int status;						// Exit status of child process
	int bal1, bal2;					// Balance read by processes
	int flag, flag1;				// End of loop variables

	int counter_dad, counter_son1, counter_son2;			/* counter variables */

	//Semaphore creation
	if((sem=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
	{
		printf("\n can't create semaphore");
		exit(1);
	}

	sem_create(sem,1);
	
	//Initialize the file balance to be $100
	fp1 = fopen("balance","w");
	bal1 = 100;
	fprintf(fp1, "%d\n", bal1);
	fclose(fp1);
	
	//Initialize the number of attempts to be 20
	fp4 = fopen("attempt", "w");
	N_Att = 20;
	fprintf(fp4, "%d\n", N_Att);
	fclose(fp4);


	// Initialize counter variables
	fp5 = fopen("counter_dad", "w");		//dad
	counter_dad = 0;
	fprintf(fp5, "%d\n", counter_dad);
	fclose(fp5);

	fp6 = fopen("counter_son1", "w");		//son1 
	counter_son1 = 0;
	fprintf(fp6, "%d\n", counter_son1);
	fclose(fp6);

	fp7 = fopen("counter_son2", "w");		//son2
	counter_son2 = 0;
	fprintf(fp7, "%d\n", counter_son2);
	fclose(fp7);

	
	//Create child processes that will do the updates
		if ((pid = fork()) == -1) 
	{
		//fork failed!
		perror("fork");
		exit(1);
	}
	
	if (pid == CHILD){
	//First Child Process. Dear old dad tries to do some updates.
	
		N=5;
		for(i=1;i<=N; i++)
		{
			P(sem);	//entering critical section
			
			// update son1
			fp6 = fopen("counter_son1","r+");
			fscanf(fp6,"%d",&counter_son1);
			fseek(fp6,0L,0);
			counter_son1++;
			fprintf(fp6, "%d\n", counter_son1);
			fclose(fp6);

			//update son2
			fp7 = fopen("counter_son2","r+");
			fscanf(fp7,"%d",&counter_son2);
			fseek(fp7,0L,0);
			counter_son2++;
			fprintf(fp7, "%d\n", counter_son2);
			fclose(fp7);

			printf("Dear old dad is trying to do update.\n");
			fp1 = fopen("balance", "r+");
			fscanf(fp1, "%d", &bal2);
			printf("Dear old dad reads balance = %d \n", bal2);
			
			//Dad has to think (0-14 sec) if his son is really worth it
			sleep(rand()%15);
			fseek(fp1,0L,0);
			bal2 += 60;
			printf("Dear old dad writes new balance = %d \n", bal2);
			fprintf(fp1, "%d \n", bal2);
			fclose(fp1);

			printf("Dear old dad is done doing update. \n");
			sleep(rand()%5);	/* Go have coffee for 0-4 sec. */

			V(sem);		//exit from critical section
		}
	}
	
	else
	{
		//Parent Process. Fork off another child process.
		if ((pid = fork()) == -1)
		{
			//Fork failed!
			perror("fork");
			exit(1);
		}
		if (pid == CHILD)
		{
			printf("First Son's Pid: %d\n",getpid());
			//Second child process. First poor son tries to do updates.
			flag = FALSE;
			while(flag == FALSE) 
			{
				P(sem);		//entering critical section

				//update dad
				fp5 = fopen("counter_dad","r+");
				fscanf(fp5,"%d",&counter_dad);
				fseek(fp5,0L,0);
				counter_dad++;
				fprintf(fp5, "%d\n", counter_dad);
				fclose(fp5);

				//update son2
				fp7 = fopen("counter_son2","r+");
				fscanf(fp7,"%d",&counter_son2);
				fseek(fp7,0L,0);
				counter_son2++;
				fprintf(fp7, "%d\n", counter_son2);
				fclose(fp7);

				fp3 = fopen("attempt" , "r+");
				fscanf(fp3, "%d", &N_Att);
				if(N_Att == 0)
				{
					fclose(fp3);
					flag = TRUE;
				}
				else
				{
					printf("Poor SON_1 wants to withdraw money.\n");
					fp2 = fopen("balance", "r+");
					fscanf(fp2,"%d", &bal2);
					printf("Poor SON_1 reads balance. Available Balance: %d \n", bal2);
					if (bal2 == 0)
					{
						fclose(fp2);
						fclose(fp3);
					}
					else
					{
						sleep(rand()%5);
						fseek(fp2,0L, 0);
						bal2 -=20;
						printf("Poor SON_1 write new balance: %d \n", bal2);
						fprintf(fp2,"%d\n", bal2);
						fclose(fp2);
						printf("poor SON_1 done doing update.\n");
						fseek(fp3,0L, 0);
						N_Att -=1;
						fprintf(fp3, "%d\n", N_Att);
						fclose(fp3);
					}
				}

				V(sem);			//exit from critical section

			}
		}
		else
		{
		//Parent Process. Fork off one more child process.
			if ((pid = fork()) == -1) 
			{
				//fork failed!
				perror("fork");
				exit(1);
			}
			if (pid == CHILD)
			{
				printf("Second Son's Pid: %d\n",getpid());
				//Third child process. Second poor son tries to do updates.
				flag1 = FALSE;
				while(flag1 == FALSE) 
				{
					P(sem);		//entering critical section

					//update dad
					fp5 = fopen("counter_dad","r+");
					fscanf(fp5,"%d",&counter_dad);
					fseek(fp5,0L,0);
					counter_dad++;
					fprintf(fp5, "%d\n", counter_dad);
					fclose(fp5);

					//update son1
					fp6 = fopen("counter_son1","r+");
					fscanf(fp6,"%d",&counter_son1);
					fseek(fp6,0L,0);
					counter_son1++;
					fprintf(fp6, "%d\n", counter_son1);
					fclose(fp6);

					fp3 = fopen("attempt" , "r+");
					fscanf(fp3, "%d", &N_Att);
					if(N_Att == 0)
					{
						fclose(fp3);
						flag1 = TRUE;
					}
					else
					{
						printf("Poor SON_2 wants to withdraw money.\n");
						fp2 = fopen("balance", "r+");
						fscanf(fp2,"%d", &bal2);
						printf("Poor SON_2 reads balance. Available Balance: %d \n", bal2);
						if (bal2 == 0)
						{
							fclose(fp2);
							fclose(fp3);
						}
						else
						{
							sleep(rand()%5);
							fseek(fp2,0L, 0);
							bal2 -=20;
							printf("Poor SON_2 write new balance: %d \n", bal2);
							fprintf(fp2,"%d\n", bal2);
							fclose(fp2);

							printf("poor SON_2 done doing update.\n");
							fseek(fp3,0L, 0);
							N_Att -=1;
							fprintf(fp3, "%d\n", N_Att);
							fclose(fp3);
						}
					}

					V(sem);		//exit from critical section
				}
			}
			else
			{
				//Now parent process waits for the child processes to finish
				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);

				fp5 = fopen("counter_dad","r+");
				fscanf(fp5,"%d",&counter_dad);
				printf("\nDad total wait counter = [%d]\n\n",counter_dad);

				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);

				fp6 = fopen("counter_son1","r+");
				fscanf(fp6,"%d",&counter_son1);
				printf("\nSon-1 total wait counter = [%d]\n\n",counter_son1);

				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);

				fp7 = fopen("counter_son2","r+");
				fscanf(fp7,"%d",&counter_son2);
				printf("Son-2 total wait counter = [%d]\n\n",counter_son2);
			}
			exit(0);
		}
		exit(0);
	}
	exit(0);
}




