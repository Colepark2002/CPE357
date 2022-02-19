#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/mman.h>
#include <string.h>

// P0:      flag[0] = true;
// P0_gate: turn = 1;
//          while (flag[1] == true && turn == 1)
//          {
//              // busy wait
//          }
//          // critical section
//          ...
//          // end of critical section
//          flag[0] = false;
// P1:      flag[1] = true;
// P1_gate: turn = 0;
//          while (flag[0] == true && turn == 0)
//          {
//              // busy wait
//          }
//          // critical section
//          ...
//          // end of critical section
//          flag[1] = false;


int main(int argc, char* argv[])
{
    int *flag = mmap(NULL,sizeof(int)*2, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    int *turn = mmap(NULL,sizeof(int), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    int *sentenceNum = mmap(NULL,sizeof(int), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    char *memArr = mmap(NULL,sizeof(char)*1000, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    *sentenceNum = 0;

    int pid;
    int ppid = getpid();
    pid = fork();
      
        
    if(pid != 0)
    {
       char prntArr[1000]; 
       while(1)
       {
            flag[0] = 1;
            *turn = 1;
            while(flag[1] == 1 && *turn == 1);
            strcpy(prntArr, memArr);
            printf("%s\n", prntArr);
            flag[0] = 0;
       }
    }
    else
    {
        char *sent1 = "A fourteen million dollar bounty on his head, and every interested party in the city wants a piece of it, i'd say the odds are about even.";
        char *sent2 = "People keep asking if I'm back and I haven't really had an answer...But now, yeah, I'm thinkin' I'm back!";
        while(1)
        {
            flag[1] = 1;
            *turn = 0;
            while(flag[0] == 1 && *turn == 0); 
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
            flag[1] = 0;
        }
        
    }  
    return 0;

}