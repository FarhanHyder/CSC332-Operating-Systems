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
        char *argv[] = {"ls","-l",NULL};    //'-l' is the extended param with 'l' 
        execvp(argv[0],argv);   //executes as "ls -l"
    }

    waitpid(child,NULL,0);  //wait for child to die :(
}