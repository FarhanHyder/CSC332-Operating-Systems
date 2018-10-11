/*  task3 : part #3 : parent
    author: Farhan Hyder
*/

#include<stdio.h>		//printf
#include <unistd.h>		//fork
#include <sys/types.h>	//fork, waitpid
#include <sys/wait.h>	//waitpid

int main(int argc, char const *argv[])
{
    pid_t child1, child2;
    int *child1_status, *child2_status;

    child1 = fork();
    if (child1 == 0)
    //child 1 process
    {
        //TODO: execute Prcs_P1 
        printf("\nchild 1\n");

        child2 = fork();
        if (child2 == 0)
        {
            //TODO: execute Prcs_P2
            printf("\nchild 2\n");
        }

    }

    return 0;
}

