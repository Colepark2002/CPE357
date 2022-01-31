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

void *prgBrk;


byte *mymalloc(unsigned int size)
{
    int i;
    unsigned int needed;
    unsigned int remaining;
    
    chunkhead *temp = head;
    chunkhead *p;
    chunkhead new;
    chunkhead *best = NULL;
    
    byte *insert;
    
    for(i = 1; ((i * PAGESIZE) - (size + sizeof(chunkhead))) < 0; i++); // finds the correct amount of pages needed
    
    needed = (i * PAGESIZE) - sizeof(chunkhead); // the needed size of memory

    if(heapsize == 0)
    {
        sbrk(i * PAGESIZE);
        prgBrk = sbrk(0);
        new.info = 1;
        new.size = needed;
        new.next = new.prev = 0;
        *(head) = new;
        heapsize += (i * PAGESIZE);
        return (byte *)(head + 1);
    }

    while(temp != 0) // finds the next available chunkhead of correct size or returns NULL
    {
        if(temp->info == 0 && temp->size >= needed)
            if(best == NULL)
                best = temp;
            else if(best->size > temp->size)
                best = temp;
            
        p = temp;
        temp = (chunkhead *)temp->next;
    }
    
    if(best != NULL) // insert if there is correctly sized free chunk 
    {
        if(best->size - needed == 0) // inserting in a chunk of exact size;
        {
            best->info == 1;
            heapsize += (i * PAGESIZE);
            return (byte *)(best + 1);
        }
        else // inserting and splitting the memory
        {
            remaining = best->size - (needed + sizeof(chunkhead));
            insert = (byte *)best;
            
            
            new.info = 0; // sets up the split chunk
            new.size = remaining - (sizeof(chunkhead));
            new.prev = insert;
            new.next = best->next;

            heapsize += best->size + sizeof(chunkhead);

            best->size = needed; // changes the open chunk to spec
            best->info = 1;
            insert = insert + sizeof(chunkhead) + best->size;
            best->next = insert; 

            temp = (chunkhead *)insert; //inserts the chunk in to memory
            *(temp) = new;
            
            

            return (byte *)(temp + 1);
        }
    }
    else // inserts at the end of the heap by extending the heap size
    {
        new.info = 1; //sets up allocated chunk
        new.size = needed;
        new.prev = p;
        new.next = 0;

        sbrk(i * PAGESIZE); // allocates memory to heap
        heapsize += (i * PAGESIZE);
        
        temp = (chunkhead*)(prgBrk + 1); // inserts the new chunkhead;
        *(temp) = new;
        prgBrk = sbrk(0);
        return (byte *)(temp + 1);

    }
    
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
            heapsize -= Prev->size + sizeof(chunkhead);
        }

        else if(Prev != 0 && Prev->info == 0)
        {
            Prev->size = temp->size + Prev->size + sizeof(chunkhead);
            Prev->next = temp->next;
            heapsize -= Prev->size + sizeof(chunkhead);
        }

        else if(Next != 0 && Next->info == 0)
        {
            temp->size = temp->size + Next->size + sizeof(chunkhead);
            temp->next = Next->next;
            heapsize += temp->size + sizeof(chunkhead);
        }
    }
    else
    {
        heapsize -= temp->size + sizeof(chunkhead);
    }

    if(heapsize == 0) // if heap has no allocated memory move program break;
    {
        brk(head);
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
    head = sbrk(0);

    byte *a,*b,*c; 
    a = mymalloc(1000); 
    b = mymalloc(1000); 
    c = mymalloc(1000); 
    myfree(b); 
    myfree(a); 
    analyse(); 
    
    return;
}