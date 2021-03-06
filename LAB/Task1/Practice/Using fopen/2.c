#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h> 
#include <errno.h>


int main(int argc, char *argv[])
{ 
    int fd;

    if(3 != argc)
    { 
        printf("\nERROR: Not Enough Arguments.\n");
        return 1;
    } 

    const char* INPUT_FILEPATH = argv[1];
    const char* OUTPUT_FILEPATH = argv[2];
    printf("Input Filepath: [%s] \nOutput Filepath: [%s]",INPUT_FILEPATH,OUTPUT_FILEPATH);
    
    errno = 0;
    fd = open(INPUT_FILEPATH,O_RDONLY);
    
    if(-1 == fd)
    { 
        printf("\nERROR : [%s] for input file.\n",strerror(errno));
        return 1;
    } 
    else 
    { 
        fd = open(OUTPUT_FILEPATH,O_RDWR);

        if(-1 == fd)
        {
            printf("\nERROR : [%s] for output file.\n",strerror(errno));
            return 1;
        }
        else
        {
            FILE *file_input, *file_output;
            
    	    char buffer_read[1000];

        	file_input =fopen(INPUT_FILEPATH,"r");
        	file_output =fopen(OUTPUT_FILEPATH,"w");

        	while (fgets(buffer_read,1000, file_input)!=NULL)
            	fprintf(file_output,"%s",buffer_read);

		    fclose(file_input);
		    fclose(file_output);
        }

        printf("\nCopy from [%s] to [%s] successful!\n",INPUT_FILEPATH,OUTPUT_FILEPATH);


    } 


    return 0;
}