#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;

    if(2 != argc)
    {
        printf("\n Usage : Enter a file name  \n");
        return 1;
    }

    errno = 0;
    fd = open(argv[1],O_CREAT|O_RDONLY);
    //fd1 = open("abc.txt",O_CREAT|O_WRONLY);
	

    if(-1 == fd)
    {
        printf("\n open() failed with error [%s]\n",strerror(errno));
        return 1;
    }
    else
    {
        write(1,"Before fork\n",12);
        char  buffer[13];
        read(fd,buffer,10);
        buffer[10]='_';
        buffer[11]='p';
        buffer[12]='\n';
        write(1,buffer,13); //1 is console
//write(fd1,buffer,13);
        //char  buffer[15];
        int rc =fork();
        if(rc<0){
            printf("Error creating the process");
        }
        else if(rc==0){
           // write(1,"here\n", 5);
            read(fd,buffer,10);
            buffer[10]='_';
            buffer[11]='c';
            buffer[12]='\n';

            write(1,buffer,13);
        }
        else{
            //char  buffer[10];
            wait(NULL);
            read(fd,buffer,10);
            buffer[10]='_';
            buffer[11]='p';
            buffer[12]='\n';

            write(1,buffer,13);
        }



    }

    return 0;
}
