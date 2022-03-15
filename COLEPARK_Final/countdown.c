#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>
float secs;

void printHelper()
{
    for(int i = 0; i < (secs * 2); i++)
    {
        fprintf(stderr,". ");
    }
    fprintf(stderr, "\n");
}

void handler(int sig)
{
    secs -= .5;
    printHelper();
    return;
}

int main(int argc, char* argv[])
{
    secs = atof(argv[1]);
    struct itimerval timer;
    timer.it_value.tv_sec = timer.it_interval.tv_sec = 0;
    timer.it_value.tv_usec = timer.it_interval.tv_usec = 500000;
    signal(SIGALRM, &handler);
    printHelper();
    setitimer(ITIMER_REAL, &timer, NULL);
    while(secs != 0);

    if(argc == 3)
    {
        if(fork() == 0)
        {
            execv(argv[2], (char*[]){argv[2], NULL});
            return 0;
        }
    }
    else
    {
        if(fork() == 0)
        {
            char *arguments[100];
            int argnum = 0;
            int index = 0;
            for(argnum = 2, index = 0; argnum < argc; argnum++, index++)
            {
                arguments[index] = argv[argnum];
            }
            arguments[index] = NULL;
            execv(argv[2], arguments);
            return 0;
        }
    }
    wait(NULL);
    return 0;
}