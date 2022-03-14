#define _GNU_SOURCE
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>
#define INPUT 100

int main(int argc, char* argv[])
{
    int size = INPUT;
    int index;
    char car;
    int programs = atoi(argv[1]);
    int *swaps = (int*)mmap(NULL,sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *Arr = (int*)mmap(NULL,sizeof(int) * size, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *Even = (int*)mmap(NULL,sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int id;
    int start;
    int end;
    int responsible;
    

    printf("\nEnter Input Array: ");
    for(index = 0;; index++)
    {
        if(index == size) // resize array if more than current size
        {
            size += INPUT;
            Arr = (int*)mremap(Arr, index, size, 0);
        }
        scanf("%d%c", &Arr[index], &car);
        
        if(car == '\n')
        {
            break;
        }
    }
    index++;
    for(int w = 0; w < index; w++)
    {
        printf("%d ", Arr[w]);
    }
    return 0;
    for(int x = 0; x < programs; x++)
    {
        if(fork()==0)
        {
            id = x;
            if(id == 0)
            {
                start = 0;
                responsible = ((index/programs) + ((index) % programs));
                end = start + responsible;
            }
            else
            {
                responsible = ((index)/programs);
                start = (id * responsible) + ((index) % programs);
                end = start + responsible;
            }
            
        }
    }
    return 0;
}