#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h> 
#include <errno.h>


int main(int argc, char *argv[])
{ 
    int fd;
    if(2 != argc)
    { 
        printf("\nERROR: Argument missing. Filepath not found!\n");
        return 1;
    } 
    
    errno = 0;
    fd = open(argv[1],O_RDONLY);
    
    if(-1 == fd)
    { 
        printf("\nERROR : [%s]\n",strerror(errno));
        return 1;
    } 
    else 
    { 
        FILE *file;
    	char buffer[1000];

    	file =fopen(argv[1],"r");

    	while (fgets(buffer,1000, file)!=NULL)
        	printf("%s",buffer);

		fclose(file);
    } 
    //fclose(fd);

    return 0;
}