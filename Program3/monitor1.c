#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/mman.h>
#include <string.h>

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
    
    int *used = mmap(NULL,sizeof(int), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    *used = 0;
    
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
            sleep(10);
            if(*used == 1)
            {
                kill(pid,9);
                *used = 0;
                munmap(used, sizeof(int));
            }
        }
        else
        {
            char input[PATH_MAX];
            int scan = scanf("%s", input);
            char ls[5] = "list";
            char *quit = "q";
            if(strcmp(input,ls) == 0)
            {
                currDir = opendir(".");
                struct dirent *file;
                while((file = readdir(currDir)) != NULL)
                {
                    printf("%s\n", file->d_name);
                }
                *used = 1;
            }
            else if(strcmp(input,quit) == 0)
            {
                return;
            }
            else
            {
                struct stat;
                int statRet = stat(input, &stat);
                if(statRet == -1)
                {
                    fprintf(stderr, "Unable to stat: %s\n", input);
                }
                else
                {
                    fprintf(stderr,"\n");
                }
                *used = 1;
            }
        }
            
    }
    return 0;
}