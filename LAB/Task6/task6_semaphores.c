/*  task6
    author: Farhan Hyder
    submission date: 12/03/2018
*/

#include <stdio.h>
#include <stdlib.h>
#include<time.h>        //time for rand generator
#include "sem.h"		//semaphore
#include <unistd.h>		//fork
#include <sys/types.h>	//kill
#include <sys/wait.h>	//kill, SIGKILL

#define PERMS 0666 		/* 0666 - To grant read and write permissions */
#define FOREVER 10      /* loop limit */

int get_rand(int start, int end);   /* generates a random number between [start,end] range */

int main()
{
    int smoker_match, smoker_paper, smoker_tobacco, agent, lock;   /*semaphore variables*/
    pid_t smoker1, smoker2, smoker3;

 /* semaphore creation [start]*/
    if((smoker_match=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
    {
        printf("\n can't create smoker_match semaphore");
        exit(1);
    }

    if((smoker_paper=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
    {
        printf("\n can't create smoker_paper semaphore");
        exit(1);
    }

    if((smoker_tobacco=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
    {
        printf("\n can't create smoker_tobacco semaphore");
        exit(1);
    }

    if((agent=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
    {
        printf("\n can't create agent semaphore");
        exit(1);
    }

    if((lock=semget(IPC_PRIVATE,1,PERMS | IPC_CREAT)) == -1)
    {
        printf("\n can't create lock semaphore");
        exit(1);
    }
    /* semaphore creation [end]*/

    /* initialize the semaphores [start] */
    sem_create(lock,1);
    sem_create(smoker_match,0);
    sem_create(smoker_paper,0);
    sem_create(smoker_tobacco,0);
    sem_create(agent,0);
    /* initialize the semaphores [end] */


    /* start main program */

    /* smokers [start] */
    smoker1=fork();
    if(smoker1==0)
    {
        int counter = 0;
        while (counter<FOREVER)
        {
            P(smoker_tobacco);    //sleep right away
            P(lock);
            printf("[smoker-3] Pick up match \n");
            printf("[smoker-3] Pick up paper \n");
            V(agent);
            V(lock);
            printf("[smoker-3] Smoke (but dont inhale) \n");
            sleep(1);
            counter ++;
        }
    }

    smoker2=fork();
    if(smoker2==0)
    {
        int counter = 0;
        while (counter<FOREVER)
        {
            P(smoker_match);    //sleep right away
            P(lock);
            printf("[smoker-1] Pick up paper \n");
            printf("[smoker-1] Pick up tobacco \n");
            V(agent);
            V(lock);
            printf("[smoker-1] Smoke (but dont inhale) \n");
            sleep(1);
            counter ++;
        }
    }

    smoker3=fork();
    if(smoker3==0)
    {
        int counter = 0;
        while (counter<FOREVER)
        {
            P(smoker_paper);    //sleep right away
            P(lock);
            printf("[smoker-2] Pick up match \n");
            printf("[smoker-2] Pick up tobacco \n");
            V(agent);
            V(lock);
            printf("[smoker-2] Smoke (but dont inhale) \n");
            sleep(1);
            counter ++;
        }
    }


    /* agent [start] */
    time_t t;
    srand((unsigned) time(&t));
    int counter = 0, randNum;

    while (counter<FOREVER)
    {
        P(lock);
        randNum = get_rand(1,3);
        printf("\n");
        if (randNum == 1)
        {
            printf("[agent] Put tobacco on table \n");
            printf("[agent] Put paper on table \n");
            V(smoker_match);

        }
        else if (randNum == 2)
        {
            printf("[agent] Put tobacco on table \n");
            printf("[agent] Put match on table \n");
            V(smoker_paper);
        }
        else if (randNum == 3)
        {
            printf("[agent] Put match on table \n");
            printf("[agent] Put paper on table \n");
            V(smoker_tobacco);
        }

        V(lock);
        P(agent);   //agent sleeps
        counter++;
    }  //end forever loop  

    /* agent [end] */


    kill(smoker1, SIGKILL);
    kill(smoker2, SIGKILL);
    kill(smoker3, SIGKILL);
    printf("\nThat's a lot of smoking for the day. Business closed! \n");

    return 0;
}


int get_rand(int start, int end)
{
    return (rand()%end) +start;
}