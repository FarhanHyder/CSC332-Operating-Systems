/*  task3 : part #3 : child 1
    author: Farhan Hyder
    submission date: 10/12/2018
*/

#include <stdio.h>      //printf      
#include <errno.h>      //errno
#include <string.h>     //strerror
#include <sys/types.h>  //open
#include <sys/stat.h>   //open
#include <fcntl.h>      //open

int main(int argc, char const *argv[])
{
    int fd;
    char *DESTINATION_1_PATH = "destination1.txt";
    char *DESTINATION_2_PATH = "destination2.txt";

    errno = 0;
    fd = open(DESTINATION_1_PATH, O_RDWR|O_CREAT,0600);
    if (fd==-1)
    //file creation/opening was not successful
    {
        printf("\nERROR : [%s][%s]\n",DESTINATION_1_PATH,strerror(errno));
    }

    errno = 0;
    fd = open(DESTINATION_2_PATH, O_RDWR|O_CREAT,0600);
    if (fd==-1)
    //file creation/opening was not successful
    {
        printf("\nERROR : [%s][%s]\n",DESTINATION_2_PATH,strerror(errno));

    }

    //printf("\nSuccess on creating and opening both destination files.\n");

    return 0;
}
