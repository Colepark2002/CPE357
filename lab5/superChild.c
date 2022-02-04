#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>


void handler(int i)
{
    fprintf(stderr,"Nice try!\n");
    return;
}


int main()
{
    int pid1, pid2;

    pid1 = getpid();
    fprintf(stderr,"Parent Process ID %d\n", pid1);
    pid2 = fork();

    signal(2,handler);
    signal(3,handler);
    signal(6,handler);
    signal(9,handler);
    signal(15,handler);
    signal(17,handler);
    signal(19,handler);
    signal(20,handler);

    if (pid2 == 0)
    { 
        for(;;)
        {
            time_t T= time(NULL); 
            struct tm currTime = *localtime(&T); //tm.tm_hour, tm.tm_min 
            fprintf(stderr, "The Time is: %d:%d:%d\n", currTime.tm_hour, currTime.tm_min, currTime.tm_sec);
            DIR *currDir = opendir(".");
            struct dirent *file;
            while((file = readdir(currDir)) != NULL)
            {
                fprintf(stderr,"%s\n", file->d_name);
            }
            closedir(currDir);
            pid1 = getpid();
            fprintf(stderr, "Child Process ID %d\n", pid1);
            sleep(10);

        }
    }
    if(pid2 > 0)
    {
        for(;;)
        {
            wait(NULL);
            pid2 = fork();
        }
    }
    return 0;
}