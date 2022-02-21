#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/mman.h>
#include <string.h>

#define true 1
#define false 0
// variables
//         wants_to_enter : array of 2 booleans
//         turn : integer

//     wants_to_enter[0] ← false
//     wants_to_enter[1] ← false
//     turn ← 0   // or 1
// p0:
//    wants_to_enter[0] ← true
//    while wants_to_enter[1] {
//       if turn ≠ 0 {
//          wants_to_enter[0] ← false
//          while turn ≠ 0 {
//            // busy wait
//          }
//          wants_to_enter[0] ← true
//       }
//    }

//    // critical section
//    ...
//    turn ← 1
//    wants_to_enter[0] ← false
//    // remainder section
// p1:
//    wants_to_enter[1] ← true
//    while wants_to_enter[0] {
//       if turn ≠ 1 {
//          wants_to_enter[1] ← false
//          while turn ≠ 1 {
//            // busy wait
//          }
//          wants_to_enter[1] ← true
//       }
//    }
 
//    // critical section
//    ...
//    turn ← 0
//    wants_to_enter[1] ← false
//    // remainder section


int main(int argc, char* argv[])
{
    int *wants_to_enter = mmap(NULL,sizeof(int)*2, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    int *turn = mmap(NULL,sizeof(int), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    int *sentenceNum = mmap(NULL,sizeof(int), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    char *memArr = mmap(NULL,sizeof(char)*1000, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    *sentenceNum = 0;
    wants_to_enter[0] = false;
    wants_to_enter[1] = false;
    *turn = 0;

    int pid;
    int ppid = getpid();
    pid = fork();
      
        
    if(pid != 0)
    {
       char prntArr[1000]; 
       while(1)
       {
            wants_to_enter[0] = true;
            while(wants_to_enter[1]);
            {
                if(*turn != 0)
                {
                    wants_to_enter[0] = false;
                    while(*turn != 0 );
                    wants_to_enter[0] = true;
                }
            }
            strcpy(prntArr, memArr);
            printf("%s\n", prntArr);
            *turn = 1;
            wants_to_enter[0] = false;
       }
    }
    else
    {
        char *sent1 = "A fourteen million dollar bounty on his head, and every interested party in the city wants a piece of it, i'd say the odds are about even.";
        char *sent2 = "People keep asking if I'm back and I haven't really had an answer...But now, yeah, I'm thinkin' I'm back!";
        while(1)
        {
            wants_to_enter[1] = true;
            while(wants_to_enter[0]);
            {
                if(*turn != 1)
                {
                    wants_to_enter[1] = false;
                    while(*turn != 1 );
                    wants_to_enter[1] = true;
                }
            } 
            if (*sentenceNum == 0)
            {
                strcpy(memArr,sent1);
                *sentenceNum = 1;
            }
            else
            {
                strcpy(memArr, sent2);
                *sentenceNum = 0;
            }
            *turn = 0;
            wants_to_enter[1] = false;
        }
        
    }  
    return 0;

}