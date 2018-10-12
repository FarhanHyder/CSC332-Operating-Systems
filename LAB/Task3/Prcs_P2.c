/*  task3 : part #3 : child 2
    author: Farhan Hyder
    submission date: 10/12/2018
*/

#include <stdio.h>      //printf
#include <sys/types.h>  //open
#include <sys/stat.h>   //open
#include <fcntl.h>      //open
#include <errno.h>      //strerror
#include <string.h>     //strerror
#include <unistd.h>     //close,read

int main(int argc, char const *argv[])
{
    int fd_source, fd_dest_1, fd_dest_2;

    char *SOURCE_PATH = "source.txt";
    char *DESTINATION_1_PATH = "destination1.txt";
    char *DESTINATION_2_PATH = "destination2.txt";
    size_t BUFFER_SIZE_1 = 100;
    size_t BUFFER_SIZE_2 = 50;
    size_t MAX_BUFFER = BUFFER_SIZE_1;
    size_t MIN_BUFFER = BUFFER_SIZE_2;


    //check read access for source file
    errno = 0;
    fd_source = open(SOURCE_PATH,O_RDONLY, 0600);
    if(fd_source==-1)
    { 
        printf("\nERROR : [%s][%s]\n",SOURCE_PATH,strerror(errno));
        return 1;
    } 

    // check write access for destination 1 file
    errno = 0;
    fd_dest_1 = open(DESTINATION_1_PATH,O_WRONLY,0600);
    if (fd_dest_1==-1)
    {
        printf("\nERROR : [%s][%s]\n",DESTINATION_1_PATH,strerror(errno));
        return 1;
    }

    // check write access for destination 2 file
    errno = 0;
    fd_dest_2 = open(DESTINATION_2_PATH,O_WRONLY,0600);
    if (fd_dest_2==-1)
    {
        printf("\nERROR : [%s][%s]\n",DESTINATION_2_PATH,strerror(errno));
        return 1;
    }


    //read (from source),replace, and store (into destination files)
    int b,fr1,fr2, counter=0;
    char buffer[MAX_BUFFER];
    do
    {
        counter++;
        if (counter%2 == 1)
        //odd counters are going into destination1
        {
            fr1 = read(fd_source,buffer,BUFFER_SIZE_1); //store the count bytes into fr
            b = fr1;            
            for (int i=0; i<fr1; i++)
            //loop through the buffer
            {
                if (buffer[i] == '1')
                    //replacing every '1' with a 'A'
                    buffer[i] = 'A';
            }
            write(fd_dest_1,buffer,fr1);
        }
        else
        //even coutners are going into destination2
        {
            fr2 = read(fd_source,buffer,BUFFER_SIZE_2); //store the count bytes into fr
            b = fr2;            
            for (int i=0; i<fr2; i++)
            //loop through the buffer
            {
                if (buffer[i] == '2')
                    //replacing every '1' with a 'A'
                    buffer[i] = 'B';
            }
            write(fd_dest_2,buffer,fr2);
        }
       
    }while(b >= MIN_BUFFER);

    //printf("Success copying data to [%d] and [%d] from [%d]",DESTINATION_1_PATH,DESTINATION_2_PATH,SOURCE_PATH);
    
    return 0;
}
