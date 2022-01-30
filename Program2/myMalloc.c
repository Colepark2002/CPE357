#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PAGESIZE 4096

typedef unsigned char byte;

typedef struct chunkhead 
{ 
    unsigned int size; 
    unsigned int info; 
    byte *next,*prev;
}chunkhead; 


chunkhead *head = NULL;
int heapsize = 0;

chunkhead *start;
void *prgBrk;


byte *mymalloc(unsigned int size)
{
    int i;
    unsigned int needed;
    chunkhead *temp = head;
    chunkhead new;
    chunkhead *best = NULL;
    
    for(i = 1; ((i * PAGESIZE) - (size + sizeof(chunkhead))) < 0; i++); // finds the correct amount of pages needed
    
    needed = (i * PAGESIZE); // the needed size of memory

    if(heapsize == 0)
    {
        prgBrk = sbrk(needed);
        new.info = 1;
        new.size = needed;
        new.next = new.prev = 0;
        *(start) = new;
        head = start;
        return (byte *)(head + 1);
    }

    while(temp != 0) // finds the next available chunkhead of correct size or returns NULL
    {
        if(temp->info == 0 && temp->size >= needed)
            best = temp;
        temp = (chunkhead *)temp->next;
    }
    
    if(best != NULL)
    {

    }

    
    return (byte *)(temp + 1); // returns the memory after the chunkhead
    
}

void myfree(byte *address)
{
    chunkhead *temp = (chunkhead *)(address - sizeof(chunkhead)); // the chunkhead's start should be at the start of memory minus the size of a chunkhead 
    
    temp->info = 0;
    
    chunkhead *Prev = (chunkhead *)temp->prev;
    chunkhead *Next = (chunkhead *)temp->next;

    if(Prev != 0 || Next != 0)
    {
        if(Prev != 0 && Prev->info == 0 && Next != 0 && Next->info == 0)
        {
            Prev->size = temp->size + Prev->size + Next->size + (2 * sizeof(chunkhead));
            Prev->next = Next->next;
        }

        else if(Prev != 0 && Prev->info == 0)
        {
            Prev->size = temp->size + Prev->size + sizeof(chunkhead);
            Prev->next = temp->next;
        }

        else if(Next != 0 && Next->info == 0)
        {
            temp->size = temp->size + Next->size + sizeof(chunkhead);
            temp->next = Next->next;
        }
    }

    return;
} 

void analyse()
{
    chunkhead *temp = head;
    int chunknum = 1;
    while(temp != 0)
    {
        printf("Chunk #%d:\n", chunknum++);
        printf("Size = %d bytes\n", temp->size);
        if(temp->info == 0)
            printf("Free\n");
        else
            printf("Occupied\n");
        printf("Next = %p\n", temp->next);
        printf("Prev = %p\n\n",temp->prev);
        temp = (chunkhead *)temp->next;
    }
    return;
}

void main()
{
    start = sbrk(0);

    byte *a,*b,*c; 
    a = mymalloc(1000); 
    b = mymalloc(1000); 
    c = mymalloc(1000); 
    myfree(b); 
    myfree(a); 
    analyse(); 
    
    return;
}