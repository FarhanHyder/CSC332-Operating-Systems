/*  task4 : #1
    author: Farhan Hyder
    submission date: 11/02/2018
    description: 
        - a special simple command interpreter that takes a command and its arguments
        - executes the comamnd
        - asks for a new command unless the user wants to exit
*/


#include<string.h>          //various string operations
#include<stdio.h>           //fork(), fgets()
#include<unistd.h>          //fork(), exec()
#include<errno.h>           //errno
#include<sys/wait.h>        //wait()
#include<stdlib.h>          //exit()


#define BUFFER_SIZE 1000
#define NULL_CHAR '\0'
#define NEW_LINE '\n'
#define TAB '\t'
#define SPACE ' '

void fix_ending(char*,char**);

     
int main()
{
    char  buffer[BUFFER_SIZE], *argv[100],last_entry;             
    int status, returnval;
    pid_t child;
     
    while (1) 
    {               
        printf("\nCommand (or 'q'/'quit' to finish): ");
     	fgets(buffer, sizeof(buffer), stdin);


	    if (buffer[strlen(buffer)-1] == NEW_LINE) 
        {
            buffer[strlen(buffer)-1] = NULL_CHAR;
        }

        fix_ending(buffer, argv);
        if (strcmp(argv[0], "q") == 0 || strcmp(argv[0], "quit") == 0)
        {
            exit(0); 
        }

        child = fork();
        errno = 0;
	    if (child < 0) 
        {    
            //printf("*** ERROR: forking child process failed\n");
            printf("\nERROR : [%s]\n",strerror(errno));
            exit(1);
	    }

	    else if (child == 0) 
        {   
            errno = 0;
            returnval = execvp(*argv, argv);
		    if (returnval < 0) 
            {    
		        // printf("*** ERROR: exec failed\n");
                printf("\nERROR : [%s]\n",strerror(errno));
		        exit(1);
		    }
	    }
	    else 
        {                                
		    while (wait(&status) != child);    
		
	    }
	}

        return 0;
}



void  fix_ending(char *ending, char **arg)
{
    while (*ending != NULL_CHAR) 
    {        
        while (*ending == SPACE || *ending == TAB || *ending == NEW_LINE)
        {
            *ending++ = NULL_CHAR;     
        }
           
        *arg++ = ending;         
        
        while (*ending != NULL_CHAR && *ending != SPACE && *ending != TAB && *ending != NEW_LINE)
        {
            ending++;  
        }

        if (*ending == NULL_CHAR)
        {
            *arg = NULL_CHAR; 
        }
                  
    }

                    
}
