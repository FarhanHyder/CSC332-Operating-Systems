 #include <unistd.h>    //execl


int main()
{
    pid_t child = fork();

    if (child==0)
    //child stuffs
    {
        
    }
    else
    //parent stuffs
    {

    }
    
}