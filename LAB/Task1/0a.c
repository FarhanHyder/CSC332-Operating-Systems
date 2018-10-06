/*  task1 : #0 (a)
    author: Farhan Hyder
    submission date: 09/21/2018
    description: Extension of provided code snippet 1. Checks for read and write access of a given file.
    run-time parameters: [1] input_file_location
    note: line 7-28 was copied and modified from the provided 'FMSysCall.pdf' by the instructor
*/

#include <stdio.h>      //printf
#include <unistd.h>     //access
#include <errno.h>      //errno, ENOENT, EACCES
#include <string.h>     //strerror

int main (int argc, char* argv[])
{ 
    char* filepath = argv[1];
    int returnval;

// Check file existence
    errno = 0;
    //if any error occurs when opening the file then it will save the error # in global var errno
    returnval = access (filepath, F_OK);    //checks file's existence
    if(returnval == 0) 
        printf ("\n\"%s\" exists!\n", filepath); 
    else 
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
    if(returnval == 0) 
        printf ("\n\"%s\" read successfull!\n", filepath); 
    else 
    { 
        printf("\nERROR : \"%s\" read not successfull! [%s]\n",filepath,strerror(errno));
        return 1;
    }

    
// Check write access
    errno = 0;  //resetting error no
    returnval = access(filepath,W_OK);  //checks write permission
    if(returnval == 0)
        printf ("\n\"%s\" write successfull!\n", filepath); 
    else
    {
        printf("\nERROR : \"%s\" write not successfull! [%s]\n", filepath,strerror(errno));
        return 1;
    }
return 0;
}