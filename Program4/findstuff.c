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

#define MAXINPUT = 4112

int *childrenArr[10];
int childInd = 0;
int pfd[2];
int childPipes[10][2];

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
    float sec;
    int h, m, s, ms;    
    sec = (end - start)/CLOCKS_PER_SEC;
	h = (sec/3600);	
	m = (sec -(3600*h))/60;
	s = (sec -(3600*h)-(m*60));
    ms = (sec - (3600*h) - (m*60) - s) * 100;
	printf("H:M:S - %d:%d:%d:%d\n",h,m,s,ms);
	
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
    

    pipe(pfd);
    char text[MAXINPUT];
    
    int realstdin = dup(STDIN_FILENO);

    for(int i = 0; i < 10; i++)
    {
        pipe(childPipes[i]);
        dup2(pfd[1], childPipes[i][0]);
        dup2(pfd[0], childPipes[i][1]);
    }

    while(1)
    {
        printf("\033[0;34m");
        printf("findstuff");
        printf("\033[0m");
        printf("$ ");
        read(STDIN_FILENO, text, MAXINPUT);
        write(pfd[1], text, MAXINPUT);
        if((childrenArr[childInd++] = fork()) == 0)
        {
            char input[MAXINPUT];
            char *args[4];
            int i = 0;
            read(childPipes[childInd-1][0], input, MAXINPUT);
            char* token = strtoK(input, " ");
            while(token != NULL)
            {
                args[i++] = token;
                token = strtok(NULL, " ");
            }
            if(strcmp(args[0], "find") == 0)
            {

            }
            else if(strcmp(args[0], "quit") == 0 || strcmp(args[0], "q") == 0)
            {
                quitMethod();
            }
            else if(strcmp([args[0], "kill"]) == 0)
            {
                int num = strtol(args[1]);
                killMethod(num);
            }
        }
        waitpid(childrenArr[childInd-1]);
    }
    
    return 0;
}