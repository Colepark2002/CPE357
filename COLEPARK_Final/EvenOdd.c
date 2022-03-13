//#include <unistd.h>
//#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//#include <sys/mman.h>
//#include <sys/wait.h>
//#include <sys/types.h>
//#include <time.h>
#define INPUT 100

int main(int argc, char* argv[])
{
    int size = INPUT;
    int i;
    char b;
    int programs = atoi(argv[1]);
    int *shared = (int*)mmap(NULL,sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *Arr = (int*)mmap(NULL,sizeof(int) * size, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *Even = (int*)mmap(NULL,sizeof(int) * size, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int id;
    int start;
    int end;
    int responsible;
    

    printf("\nEnter Input Array: ");
    for(i = 0;; i++)
    {
        if(i == size) // resize array if more than current size
        {

        }
        scanf("%d%c", &Arr[i], &b);
        
        if(b == '\n')
        {
            break;
        }
    }
    for(int w = 0; w <= i; w++)
    {
        printf("%d ", Arr[w]);
    }
    for(int x = 0; x < programs; x++)
    {
        if(fork()==0)
        {
            id = x;
        }
    }
    return 0;
}