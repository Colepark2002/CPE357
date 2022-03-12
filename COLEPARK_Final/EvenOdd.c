//#include <unistd.h>
//#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//#include <sys/mman.h>
//#include <sys/wait.h>
//#include <sys/types.h>
//#include <time.h>


int main(int argc, char* argv[])
{
    int c[100];
    int i;
    char b;
    printf("\nEnter Input Array: ");
    for(i = 0; scanf("%d%c", &c[i], &b); i++)
    {
        printf("%d ", c[i]);
        if(b == '\n')
        {
            break;
        }
    }
    return 0;
}