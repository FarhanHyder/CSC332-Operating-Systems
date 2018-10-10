/*  task3 : part #2
    author: Farhan Hyder
*/

#include <unistd.h>     //execvp
#include <stdio.h>      //printf
#include <sys/types.h>  //waitpid
#include <sys/wait.h>   //waitpid


int main(int argc, char *argv[])
{
    printf("Parent PID #[%d]\n",getpid());

    pid_t child = fork();    
    if (child==0)
    //child stuffs
    {
        printf("Child PID #[%d]\n",getpid());
        execvp("ls -l", &argv[0]);
    }

    waitpid(child,NULL,0);  //wait for child to die :(
}