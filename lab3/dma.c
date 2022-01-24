#include <stdio.h>
#include <stdlib.h>

#define PAGESIZE 1024

typedef unsigned char byte;

typedef struct chunkhead 
{ 
    unsigned int size; 
    unsigned int info; 
    byte *next,*prev;
}chunkhead; 

byte myheap[1048576];

chunkhead *head = (chunkhead *)myheap;


byte *mymalloc(unsigned int size)
{
    int i;
    unsigned int needed;
    unsigned int remaining;
    chunkhead *temp = head;
    chunkhead rest;
    
    for(i = 1; ((i * PAGESIZE) - size) < 0; i++); // finds the correct amount of pages needed
    
    needed = (i * PAGESIZE); // the needed size of memory

    while(temp->info != 0 || temp->size < needed) // finds the next available chunkhead of correct size or returns NULL
    {
        if(temp->next == 0)
            return NULL;
        temp = (chunkhead *)temp->next;
    }

    remaining = temp->size - needed; // if inserting at the end of the heap the remaining space is the size of the heap minus our need size of the malloc 

    if(temp->next == 0 && remaining > sizeof(chunkhead)) // if there is room at the end of the heap, make a new chunkhead for the unallocated memory
    {
        
        rest.size = remaining - sizeof(chunkhead);
        rest.info = 0;
        rest.next = 0;
        rest.prev = (byte *)temp;
        temp->size = needed;
        temp->next = (byte *)(temp + sizeof(chunkhead) + temp->size); // the next chunkhead should be our new chunkhead for the unallocated memory
        *(temp + sizeof(chunkhead) + temp->size) = rest;
    }
    
    temp->info = 1; // set to occupied
    
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
    head->info = 0;
    head->size = 1048576 - sizeof(chunkhead);
    head->next = head->prev = 0;

    byte *a,*b,*c; 
    a = mymalloc(1000); 
    b = mymalloc(1000); 
    c = mymalloc(1000); 
    myfree(b); 
    myfree(a); 
    analyse(); 
    
    return;
}