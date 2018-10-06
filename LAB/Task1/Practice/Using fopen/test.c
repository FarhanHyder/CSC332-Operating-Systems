#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main (int argc, char* argv[])
{ 
    //char comparison
    // char a = '1';
    // char b = '1';

    // if(a==b)
    //     printf("Same!\n");

    FILE *file_input;
    char *INPUT_FILEPATH = "source.txt";

            int counter = 0;
            int c;
            char *str = malloc(5000);

        	file_input =fopen(INPUT_FILEPATH,"r");

            while ((c = fgetc(file_input)) != EOF)  //reading the file char by char
            {
                counter++;
            }

            printf("Total characters in [%s] = %d",INPUT_FILEPATH,counter);

    return 0;
}