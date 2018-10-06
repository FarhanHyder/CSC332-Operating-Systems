/*  task1: #0 (b)
    author: Farhan Hyder
    submission date: 09/21/2018
    description: open() system call creates a new file (say: destination.txt) and then opens it using a bitwise OR flag.
    run-time parameters: [1] destination_file_location
*/

#include <sys/types.h>  //open
#include <sys/stat.h>   //open
#include <fcntl.h>      //open
#include <errno.h>      //strerror
#include <unistd.h>     //close
#include <stdio.h>      //printf
#include <string.h>     //string, strerror

int main(int argc, char *argv[])
{ 
    int fd;

    // check arguments
    if(argc != 2)
    //arg counter must be 2. otherwise the program has too many or too less args
    { 
        if (argc < 2)
            printf("[ERROR]: Not enough arguments!\n");
        else
            printf("[ERROR]: Too many arguments!\n");
        return 1;
    } 
    
    errno = 0;
    //if any error occurs when opening the file then it will save the error # in global var errno
    fd = open(argv[1],O_RDONLY|O_CREAT); //creates and open in read mode
        //note: created file will be a read-only file
    
    if(fd == -1)
    { 
        printf("\nFile \"%s\" creation and open failed with error [%s]\n",argv[1],strerror(errno));
        return 1;
    } 
    else 
    { 
        printf("\nFile \"%s\" creation and open successful!\n",argv[1]);
    } 
    close(fd);  //closing the read mode

    return 0;
}