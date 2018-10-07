#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    int x = 0;
    int rc = fork();
    if(rc<0){
        printf("error creating child process");
    }
    else if(rc==0){
        x++;

        printf("I am a child process and my pid id : %u\n",getpid());
        printf("The value of x in child is %d: \n",x);

    }
    else{
        x=2;
        printf("I am a parent with pid %u waiting my child to die\n",getpid());
        int rc_wait=wait(NULL);
        printf("I am a parent  and my child just died!!\nAnd i am next to die bye!\n");
        printf("The value of x in parent is %d : \n",x);

    }
    printf("The value of x at the end is %d : \n",x);

}