/*  task4 : #2
    author: Farhan Hyder
    submission date: 11/02/2018
*/

#include<string.h>      //various string operations
#include<stdio.h>       //fork(),fscanf()
#include<errno.h>       //errno
#include<unistd.h>      //F_OK,R_OK    
#include<stdlib.h>      //exit()
#include<sys/wait.h>    //wait()

int main(int argc, char* argv[])
{


    int n_students = 10;    //rows
    int x=2, y=2;  
    int row_n=0, col_n=0, worker_n = 0;
    double grade_total_temp; 
    int status, returnval;
    char* SOURCE_FILEPATH = "quiz_grades.txt";
    pid_t manager, worker;
    double grades[x*y][n_students];

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
    FILE *fd=fopen(SOURCE_FILEPATH,"r");

    while(row_n <= n_students-1)
    {
        while(col_n <= ((x*y))-1)
        {
            fscanf(fd,"%lf", &grades[col_n][row_n]);
            col_n++;
        }
        col_n=0;
        row_n++;
    }

    for (int i=0; i<x; i++)
    //x = number of managers
    {
        manager = fork();
        if (manager==0)
        {
            for (int j =0; j<y; j++)
            //j = number of workers
            {
                worker = fork();
                if (worker == 0 )
                {
                    worker_n = (i*x)+j;
                    for(int k=0; k < n_students; k++)
		            {
		                grade_total_temp += grades[worker_n][k];
		            }
                    
                    worker_n++;
                    printf("Avg Grade is: %lf \t\t[graded by worker#%d under manager#%d]\n", (grade_total_temp/(n_students)),(worker_n),(i+1));
	                wait(&status);
                    exit(0); 
                }
                else if(worker == -1) 
                {
                    printf("ERROR: Worker was not created successfully!");     
                }
                else 
                {
                    wait(&status);  
                }

            }
            wait(&status);
                    exit(0);
        }
        else if (manager==-1)
        {
            printf("ERROR: Manager WAS NOT created successfully!");     

        }
        else
        {
            wait(&status);  
        }
    }

    fclose(fd);
    return 0;
}
