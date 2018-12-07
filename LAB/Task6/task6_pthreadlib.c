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
#include<pthread.h>     //pthread lib

#define FOREVER 10      /* loop limit */
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t smoker_match =PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t smoker_paper=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t smoker_tobacco=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t agent=PTHREAD_MUTEX_INITIALIZER;

int get_rand(int, int);
void *agent_fn(void*);
void *smoker_fn(void*);

void main()
{
    pthread_t agent_thread, smoker1, smoker2, smoker3;

    /* init mutex variables */
    pthread_mutex_init(&lock,NULL);
    pthread_mutex_init(&smoker_match,NULL);
    pthread_mutex_init(&smoker_paper,NULL);
    pthread_mutex_init(&smoker_tobacco,NULL);
    pthread_mutex_init(&agent,NULL);
    
    /* everything but lock should be locked */
    pthread_mutex_unlock(&lock);
    pthread_mutex_lock(&smoker_match);
    pthread_mutex_lock(&smoker_paper);
    pthread_mutex_lock(&smoker_tobacco);
    pthread_mutex_lock(&agent);

    pthread_create(&agent_thread,NULL, &agent_fn,NULL);
    pthread_create(&smoker1,NULL, smoker_fn,(void*)1);
    pthread_create(&smoker2,NULL, smoker_fn,(void*)2);
    pthread_create(&smoker3,NULL, smoker_fn,(void*)3);

   pthread_join(agent_thread, NULL);
   pthread_kill(smoker1,SIGTERM);
   pthread_kill(smoker2,SIGTERM);
   pthread_kill(smoker3, SIGTERM);

   exit(EXIT_SUCCESS);

}



void *agent_fn(void *arg)
{
    time_t t;
    srand((unsigned) time(&t));
    int counter = 0, randNum;

    while (counter<FOREVER)
    {
        pthread_mutex_lock(&lock);
        randNum = get_rand(1,3);
        printf("\n");
        if (randNum == 1)
        {
            printf("[agent] Put tobacco on table \n");
            printf("[agent] Put paper on table \n");
            pthread_mutex_unlock(&smoker_match);

        }
        else if (randNum == 2)
        {
            printf("[agent] Put tobacco on table \n");
            printf("[agent] Put match on table \n");
            pthread_mutex_unlock(&smoker_paper);

        }
        else if (randNum == 3)
        {
            printf("[agent] Put match on table \n");
            printf("[agent] Put paper on table \n");
            pthread_mutex_unlock(&smoker_tobacco);
        }

        pthread_mutex_unlock(&lock);
        pthread_mutex_lock(&agent);
        counter++;
    }  //end forever loop  

}


void *smoker_fn(void *arg)
{
    int n = (long) arg;

    if(n==1)    // smoker#1
    {
        int counter = 0;
        while (counter<FOREVER)
        {
            pthread_mutex_lock(&smoker_tobacco);    //sleep right away
            pthread_mutex_lock(&lock);
            printf("[smoker-3] Pick up match \n");
            printf("[smoker-3] Pick up paper \n");
            pthread_mutex_unlock(&agent);
            pthread_mutex_unlock(&lock);
            printf("[smoker-3] Smoke (but dont inhale) \n");
            sleep(1);
            counter ++;
        }
    }

    if(n==2)
    {
        int counter = 0;
        while (counter<FOREVER || counter < 0)
        {
            pthread_mutex_lock(&smoker_match);  //sleep right away
            pthread_mutex_lock(&lock);
            printf("[smoker-1] Pick up paper \n");
            printf("[smoker-1] Pick up tobacco \n");
            pthread_mutex_unlock(&agent);
            pthread_mutex_unlock(&lock);
            printf("[smoker-1] Smoke (but dont inhale) \n");
            sleep(1);
            counter ++;
        }
    }

    if(n==3)
    {
        int counter = 0;
        while (counter<FOREVER || counter < 0)
        {
            pthread_mutex_lock(&smoker_paper);  //sleep right away
            pthread_mutex_lock(&lock);
            printf("[smoker-2] Pick up match \n");
            printf("[smoker-2] Pick up tobacco \n");
            pthread_mutex_unlock(&agent);
            pthread_mutex_unlock(&lock);
            printf("[smoker-2] Smoke (but dont inhale) \n");
            sleep(1);
            counter ++;
        }
    }
}


int get_rand(int start, int end)
{
    return (rand()%end) +start;
}