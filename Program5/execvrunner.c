#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char* argv[])
{
    int programs = atoi(argv[2]);
    
    for(int i = 0; i < programs; i++)
    {
        if(fork() == 0)
        {
            execv(argv[1], (char*[]){argv[1], i, programs, NULL});
            return 0;
        }
        else
        {
            wait(NULL);
        }
        
    }
}