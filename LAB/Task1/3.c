/*  task1 : #3
    author: Farhan Hyder
    submission date: 09/21/2018
    description:  A program that mimics the 'cp' command using the open() system call.
        opens source file in read-only mode and copy the contents of it to destination file,
            using read() and write() system calls. 
                -> replace each char '1' with char 'A'
                -> write 'XYZ' after each 100 char count
                -> end with a 'XYZ' if not ended before 
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
    int ending_with_xyz;
    do{
        ending_with_xyz = 0;
        fr = read(fd_reader,buffer,BUFFER_SIZE); //store the count bytes into fr
        for (int i=0; i<fr; i++)
        //loop through the buffer
        {
            if (buffer[i] == '1')
                //replacing every '1' with a 'A'
                buffer[i] = 'A';
        }
        write(fd_writer,buffer,fr);   //finally :) write to the destination file
        if(fr == 100)
        {
            write(fd_writer,"XYZ",3);   //inserting "XYZ" after every 100 chars
            ending_with_xyz = 1;
        }
    }while(fr >= BUFFER_SIZE);
    
    if (ending_with_xyz == 0 && fr != 0)
    //fr=0 if source_file has total char counts that %100 == 0 (1200,1300)
    {
        write(fd_writer,"XYZ",3);   //ending the file with a 'XYZ' if not done before
    }
    close(fd_reader);  //closing open file
    close(fd_writer);  //closing write file

    return 0;
}