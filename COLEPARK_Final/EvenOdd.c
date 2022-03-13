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
    int index;
    char b;
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
            Arr = (int*)mremap(Arr, size, size+=INPUT);
        }
        scanf("%d%c", &Arr[index], &b);
        
        if(b == '\n')
        {
            break;
        }
    }
    for(int w = 0; w <= index; w++)
    {
        printf("%d ", Arr[w]);
    }
    for(int x = 0; x < programs; x++)
    {
        if(fork()==0)
        {
            id = x;
            if(id == 0)
            {
                start = 0;
                responsible = ((index + 1)/programs) + ((index +1) % programs);
                end = start + responsible;
            }
            else
            {
                responsible = ((index + 1)/programs);
                start = (id * responsible) + ((index +1) % programs);
                end = start + responsible;
            }
            
        }
    }
    return 0;
}