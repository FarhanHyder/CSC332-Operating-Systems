/*  task3 : part #3 : parent
    author: Farhan Hyder
    submission date: 10/12/2018
*/

#include <unistd.h>     //execv
#include<stdio.h>		//printf
#include <unistd.h>		//fork
#include <sys/types.h>	//fork, waitpid
#include <sys/wait.h>	//waitpid

int main(int argc, char *const *argv[])
{
    pid_t child1, child2;
    int *child1_status, *child2_status;

    child1 = fork();

    if (child1 == 0)
    //child 1 process
    {
        //TODO: execute Prcs_P1
        printf("\n[Attempt] Create and open two destination files!\n");
        execl("./Prcs_P1","Prcs_P1",NULL);
    }
    else
    {
        printf("\n[Success] Create and open two destination files!\n");
       
        sleep(1);

        printf("\n[Attempt] Copy modified data to destination files from source file!\n");
        child2 = fork();
        if (child2 == 0)
        {
            //TODO: execute Prcs_P2
            execl("./Prcs_P2","Prcs_P2",NULL);
        }
        printf("\n[Success] Copy modified data to destination files from source file!\n");


    }

    return 0;
}

