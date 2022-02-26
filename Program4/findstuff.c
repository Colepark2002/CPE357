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
#include <time.h>


int *childrenArr[10];

void searchDir(char* search, char* dirname, int index, char** paths)
{
    DIR* dir = opendir(dirname);
    struct dirent *file;
    struct stat sb;
    while((file = readdir(currDir)) != NULL)
    {
        stat(file->dname, &sb);
        if(S_ISDIR(sb.st_mode))
        {
            searchDir(search, file->dname, index++, paths);
        }
        if(strcmp(search, file->dname) == 0)
        {
            paths[index++] = file->dname;
        }
    }
}

void find(char* search, int s, char* filetype)
{
    time_t start = time();

    if(search[0] == '"')
    {
        if(s)
        {

        }
        else
        {

        }
    }

    else
    {
        char *paths[PATH_MAX];
        int i = 0;
        if(s)
        {
            searchDir(search,".", i, paths);
        }
        else
        {
            
            DIR* dir = opendir(".");
            struct dirent *file;
            while((file = readdir(currDir)) != NULL)
            {
                if(strcmp(search, file->dname) == 0)
                {
                    paths[i++] = file->dname;
                }
            }
        }

    }

    time_t end = time();
    int sec, h, m, s;    
    sec = (end - start)/CLOCKS_PER_SEC;
	h = (sec/3600);	
	m = (sec -(3600*h))/60;
	s = (sec -(3600*h)-(m*60));
	printf("H:M:S - %d:%d:%d\n",h,m,s);
	
	return;
}

void killMethod(int childNum)
{
    kill(childrenArr[childNum-1], 2);
}

void quitMethod()
{
    for(int i = 0; i < 10; i++)
    {
        if(childrenArr[i] != 0)
        {
            killMethod(i+1);
        }
    }
}









int main()
{
    printf("\033[0;34m");
    printf("findstuff");
    printf("\033[0m");
    printf("$ ");
    return 0;
}