#define _GNU_SOURCE
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#define INPUT 10



void synch(int id, int par_count, int* ready)
{
    int newVal = ++ready[id];
    for(int i = 0; i < par_count; i++)
    {
        while(ready[i] < newVal);
    }
    return;
}

int main(int argc, char* argv[])
{
    int pid;
    int mapsize = INPUT; // size of the mapped array
    int ArrSize; // array size
    char car; //used to find \n
    int programs = atoi(argv[1]); // program count
    if (programs > (ArrSize/2)) // if you have too many programs reset
    {
        programs = ArrSize/2;
    }
    int *swaps = (int*)mmap(NULL,sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0); // Counts amount of swaps
    int *Arr = (int*)mmap(NULL,sizeof(int) * mapsize, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0); // Shared Array
    int *ready = (int*)mmap(NULL, sizeof(int) * programs, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1,0); //used for synch
    memset(ready, 0, sizeof(int) * programs);
    int id; // process id
    int start; // start index
    int end; // end index
    int responsible; // how many integers responsible for in array
    
    
    printf("\nEnter Input Array: ");
    for(ArrSize = 0;; ArrSize++) // infinitely reads from input until hitting a \n
    {
        if(ArrSize == mapsize) // resize array if more than current size
        {
            mapsize += INPUT;
            Arr = (int*)mremap(Arr, ArrSize, mapsize, 0);
        }
        scanf("%d%c", &Arr[ArrSize], &car);
        
        if(car == '\n')
        {
            break;
        }
    }
    printf("\n");
    ArrSize++;
    
    printf("Initial Array: ");
    for(int w = 0; w < ArrSize; w++) // prints array
    {
        printf("%d ", Arr[w]);
    }
    printf("\n");
    *swaps = 1;
    for(int x = 0; x < programs; x++) // forks and initializes the children
    {
        if((pid = fork())==0)
        {
            id = x;
            if(id == 0)
            {
                start = 0;
                responsible = ((ArrSize/programs) + ((ArrSize) % programs));
                end = start + responsible;
                break;
            }
            else
            {
                responsible = ((ArrSize)/programs);
                start = (id * responsible) + ((ArrSize) % programs);
                end = start + responsible;
                break;
            }
            
        }
    }
    time_t starter = clock();
    if (pid == 0)
    {
        while(*swaps != 0) // sorting algorithm
        {
            synch(id, programs, ready);
            if(id == 0)
                *swaps = 0;
            for(int i = start; i < start + responsible && i < ArrSize - 1; i+=2) // Even Sort
            {
                if(Arr[i] > Arr[i+1])
                {
                    int temp = Arr[i];
                    Arr[i] = Arr[i+1];
                    Arr[i+1] = temp;
                    *swaps += 1;
                }
            }
            synch(id, programs, ready);
            for(int i = start + 1; i < start + responsible && i < ArrSize - 1; i+=2) // Odd Sort
            {
                if(Arr[i] > Arr[i+1])
                {
                    int temp = Arr[i];
                    Arr[i] = Arr[i+1];
                    Arr[i+1] = temp;
                    *swaps += 1; 
                }
            }
            
        }
    }
    else
    {
        wait(NULL);
        time_t ender = clock();
        double time = ender - starter;
        printf("Sorted Array: ");
        for(int w = 0; w < ArrSize; w++) // prints array
        {
            printf("%d ", Arr[w]);
        }
        printf("\n\nProcesses: %d\nTime to Sort: %f secs\n", programs, time/CLOCKS_PER_SEC);
        munmap(swaps, sizeof(int));
        munmap(ready, sizeof(int)*programs);
        munmap(Arr,sizeof(int)*mapsize);
    }
    

    return 0;
}