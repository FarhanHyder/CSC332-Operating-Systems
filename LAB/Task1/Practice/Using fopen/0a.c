#include <stdio.h>
#include <unistd.h> 
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main (int argc, char* argv[])
{ 
    char* filepath = argv[1];
    int returnval;

// Check file existence 

    returnval = access (filepath, F_OK);
    if(returnval == 0) 
        printf ("\n%s exists\n", filepath); 
    else 
    { 
        if(errno == ENOENT) 
            printf ("%s does not exist\n", filepath);
        else if (errno == EACCES) 
            printf ("%s is not accessible\n",filepath);
        return 0;
    }

// Check read access ...
    returnval = access(filepath,R_OK);
    if(returnval == 0) 
        printf ("\n%s READ successfully!\n", filepath); 
    else 
    { 
        printf("\n%s COULD NOT BE READ successfully!\n", filepath);
        return 0;
    }

    
// Check write access ...
    returnval = access(filepath,R_OK);
    if(returnval == 0) 
        printf ("\nWRITE successfull in the file ->%s.\n", filepath); 
    else 
    { 
        printf("\nWRITE WAS NOT successfull in the file ->%s\n", filepath);
    }

return 0;
}