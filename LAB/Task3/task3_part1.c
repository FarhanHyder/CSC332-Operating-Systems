/*  task3 : part #1
    author: Farhan Hyder
    submission date: 10/12/2018
    description: 
		- creates a child that executes a command which tells time and date in Unix
*/

#include <unistd.h>    //execl
#include <stdio.h>     //printf
#include <sys/types.h> //waitpid
#include <sys/wait.h>  //waitpid


int main(int argc, char *argv[])
{
    printf("Parent PID #[%d]\n",getpid());

    pid_t child = fork();    
    if (child==0)
    //child stuffs
    {
        printf("Child PID #[%d]\n",getpid());
        execl("/bin/date", argv[0],NULL);
    }
    
    waitpid(child,NULL,0);  //wait for child to die :(

}