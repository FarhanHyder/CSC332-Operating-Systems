//
// Created by prajwal on 9/6/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    int pid;
    int status;
    int count=0;
    int rc, i=0;
    printf("I am a parent. My pid is %d\n",getpid());
    while ((++count)<=10 && (rc=fork()));

    if(rc==0){
        printf("I am the %d th child. My pid is %d and my parent is %d\n",count, getpid(),getppid());
    }
    else if(rc>0){
        printf("My pid is %d I am waiting for my children to die\n",getpid());
        while ((pid=waitpid(-1,&status,0))!=-1) {
            printf("Process %d terminated\n",pid);
        }
    }

}