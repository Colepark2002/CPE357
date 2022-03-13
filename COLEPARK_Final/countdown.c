//#include <unistd.h>
//#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//#include <sys/mman.h>
//#include <sys/wait.h>
//#include <sys/types.h>
//#include <sys/time.h>
float secs

void printHelper()
{
    for(int i = 0; i < (secs * 2); i++)
    {
        printf(". ");
    }
    if(secs)
    {
        printf("// %f seconds until START\n", secs);
    }
    else
    {
        printf("// Start of other program\n");
    }
}

void alarm(int sig)
{
    secs -= .5;
    printHelper();
    return;
}

int main(int argc, char* argv[])
{
    secs = atof(argv[1]);
    printf("%f\n", secs);
    struct itimerval timer;
    timer.it_value.tv_sec = timer.it_interval.tv_sec = 0;
    timer.it_value.tv_usec = timer.it_interval.tv_usec = 500000;
    signal(SIGALRM, &alarm);
    printHelper();
    setitimer(ITIMER_REAL, timer, NULL);
    while(secs != 0);

    if(argc > 3)
    {
        if(fork() == 0)
        {
            execv(argv[2], NULL);
            return;
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
            return;
        }
    }
    wait(NULL);
    return 0;
}