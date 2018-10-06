/*  task1 : #1
    author: Farhan Hyder
    submission date: 09/21/2018
    description: takes a (text) file name as argument and display it's content.
        Reports appropriate message if the file does not exist or file cannot be opened.
    run-time parameters: [1] input_text_file
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
    const size_t BUFFER_SIZE = 100;

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
    
    char* filepath = argv[1];
    
// Check file existence 
    errno = 0;
    //if any error occurs when opening the file then it will save the error # in global var errno
    returnval = access (filepath, F_OK);    //checks file's existence
    if(returnval != 0) 
    { 
        if(errno == ENOENT) 
            printf ("ERROR: \"%s\" does not exist! [%s]\n", filepath,strerror(errno));
        else if (errno == EACCES) 
            printf ("ERROR: \"%s\" is not accessible! [%s]\n",filepath,strerror(errno));
        return 1;
    }

// Check read access
    errno = 0;  //resetting error no
    returnval = access(filepath,R_OK);  //checks read permission
    if(returnval != 0) 
    { 
        printf("\nERROR : \"%s\" read not successfull! [%s]\n",filepath,strerror(errno));
        return 1;
    }

//opening file for reading
    errno = 0;  //resetting error no
    fd = open(filepath,O_RDONLY);
    if(fd == -1)
    { 
        printf("\nERROR : [%s]\n",strerror(errno));
        return 1;
    } 

//else read(store) into the buffer and display
    int fr;
    char buffer[BUFFER_SIZE];
    printf("displaycontent command on file \"%s\": \n",filepath);

    do{
        fr = read(fd,buffer,BUFFER_SIZE); //storing the count bytes into fr
        //loop through the buffer to print
        for (int i=0; i<fr; i++)
            //this loops guarantees that code will not print any garbage values
            printf("%c",buffer[i]);
    }while(fr>=BUFFER_SIZE);    //if loop has a byte count that is less than BUFFER_SIZE, it stops

    close(fd);  //closing open file
    

    return 0;
}