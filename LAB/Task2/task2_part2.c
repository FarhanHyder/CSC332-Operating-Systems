/*  task2 : part #2
    author: Farhan Hyder
    submission date: 10/05/2018
    description: 
		- parent P forks process Q
        	- process Q(now parent) forks process R
*/

#include <stdio.h>      //printf
#include <sys/types.h>  //fork, getpid
#include <unistd.h>     //fork, getpid

int main(int argc, char const *argv[])
{
    //parent P
    int a=10, b=25;
    pid_t fq=0, fr=0;
    fq = fork();    //fork a child - call it process Q

    if (fq==0)
    //child successfully forked [Q]
    {
        a = a+b;
        printf("\n\na= %d, b= %d, process_id= %d",a,b,getpid());

        fr = fork();    //fork another child, call it process R
        if (fr!=0)
        {
            b = b+20;
            printf("\na= %d, b= %d, process_id= %d\n",a,b,getpid());
        }else
        //child successfully forked [R] 
        {
            a = (a*b)+30;
            printf("\na= %d, b= %d, process_id= %d\n",a,b,getpid());
        }
    
    }else
    {
        b = a+b-5;
        printf("a= %d, b= %d, process_id= %d",a,b,getpid());
    }

    return 0;
}
