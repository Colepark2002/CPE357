#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/wait.h>
#include <signal.h>

void handler(int i)
{
    fprintf(stderr,"\nNice try!\n");
    return;
}


int main()
{
    DIR *currDir;
    int pid;
    pid = fork();
    
    if(pid != 0)
    {
        fprintf(stderr, "Parent PID: %d\n", getpid());
    }
    
    

    while(1)
    {
        signal(2,handler);
        signal(3,handler);
        signal(6,handler);
        signal(9,handler);
        signal(15,handler);
        signal(18,handler);
        signal(19,handler);
        signal(20,handler);
        
        if(pid != 0)
        {
            wait(NULL);
            pid = fork();
        }
        else
        {
            time_t T = time(NULL); 
            struct tm currTime = *localtime(&T); //tm.tm_hour, tm.tm_min 
            printf("The Time is: %d:%d:%d\n", currTime.tm_hour, currTime.tm_min, currTime.tm_sec);
            currDir = opendir(".");
            struct dirent *file;
            while((file = readdir(currDir)) != NULL)
            {
                printf("%s\n", file->d_name);
            }
            printf("Child Process ID %d\n", getpid());
            sleep(10);
        }
            
    }
    return 0;
}