/*  task1 : #2
    author: Farhan Hyder
    submission date: 09/21/2018
    description:  A program that mimics the 'cp' command using the open() system call.
        opens source file in read-only mode and copy the contents of it to destination file,
            using read() and write() system calls. 
    run-time parameters: [1] source_file [2] destination_file
*/



#include <stdio.h>      //printf
#include <sys/types.h>  //open
#include <sys/stat.h>   //open
#include <fcntl.h>      //open
#include <errno.h>      //strerror
#include <unistd.h>     //close,read
#include <string.h>     //string, strerror

int main(int argc, char *argv[])
{ 
    int fd, returnval;
    size_t BUFFER_SIZE = 100;

// check arguments
    if(argc != 3)
    //arg counter must be 3. otherwise the program has too many or too less args
    { 
        if (argc < 3)
            printf("[ERROR]: Not enough arguments!\n");
        else
            printf("[ERROR]: Too many arguments!\n");
        return 1;
    }

    char *SOURCE_FILEPATH = argv[1];
    char* DESTINATION_FILEPATH = argv[2];
    
// Check file existence 
    returnval = access (SOURCE_FILEPATH, F_OK);    //checks file's existence
    if(returnval != 0) 
    { 
        if(errno == ENOENT) 
            printf ("ERROR: \"%s\" does not exist! [%s]\n", SOURCE_FILEPATH,strerror(errno));
        else if (errno == EACCES) 
            printf ("ERROR: \"%s\" is not accessible! [%s]\n",SOURCE_FILEPATH,strerror(errno));
        return 1;
    }

// Check read access ...
    returnval = access(SOURCE_FILEPATH,R_OK);  //checks read permission
    if(returnval != 0) 
    { 
        printf("\nERROR : \"%s\" read not successfull! [%s]\n",SOURCE_FILEPATH,strerror(errno));
        return 1;
    }

    errno = 0;

//open source file for reading with error checking
    int fd_reader = open(SOURCE_FILEPATH,O_RDONLY, 0600);
    if(fd_reader == -1)
    { 
        printf("\nERROR : [%s]\n",strerror(errno));
        return 1;
    } 

//open destination file for writing with error checking
    int fd_writer = open(DESTINATION_FILEPATH,O_WRONLY | O_CREAT, 0600);
    if(fd_writer == -1)
    {
        printf("\nERROR : [%s]\n",strerror(errno));
        return 1;
    } 

//read(store) into the buffer
    int fr;
    char buffer[BUFFER_SIZE];

    do{
        fr = read(fd_reader,buffer,BUFFER_SIZE); //store the count bytes into fr
        write(fd_writer,buffer,fr);   //finally :) //write to the destination file
    }while(fr >= BUFFER_SIZE); //if loop has a byte count that is less than BUFFER_SIZE, it stops


    close(fd_reader);  //closing reader
    close(fd_writer);  //closing writer

    return 0;
}