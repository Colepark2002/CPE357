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
    
    for(i = 1; (i * PAGESIZE) < (size + sizeof(chunkhead)); i++); // finds the correct amount of pages needed
    
    needed = (i * PAGESIZE) - sizeof(chunkhead); // the needed size of memory

    if(heapsize == 0)
    {
        head = sbrk(0);
        sbrk(i * PAGESIZE);
        prgBrk = sbrk(0);
        new.info = 1;
        new.size = needed;
        new.next = new.prev = 0;
        *(head) = new;
        heapsize += (i * PAGESIZE);
        return (byte *)(head + 1);
    }

    while(temp != 0) // finds the best available chunkhead of correct size
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
            remaining = best->size - (i * PAGESIZE);
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
        new.prev = (byte *)p;
        new.next = 0;

        sbrk(i * PAGESIZE); // allocates memory to heap
        heapsize += (i * PAGESIZE);
        
        temp = (chunkhead*)(prgBrk); // inserts the new chunkhead;
        *(temp) = new;
        prgBrk = sbrk(0);
        p->next = (byte *)temp;
        return (byte *)(temp + 1);

    }
    
}

void myfree(byte *address)
{
    chunkhead *temp = (chunkhead *)(address - sizeof(chunkhead)); // the chunkhead's start should be at the start of memory minus the size of a chunkhead 
    
    temp->info = 0;
    
    chunkhead *Prev = (chunkhead *)temp->prev;
    chunkhead *Next = (chunkhead *)temp->next;


    if(Prev != 0 || Next != 0) // if prev or next exists 
    {
        if(Prev != 0 && Prev->info == 0 && Next != 0 && Next->info == 0) // if both are free
        {
            Prev->size = temp->size + Prev->size + Next->size + (2 * sizeof(chunkhead));
            Prev->next = Next->next;
            heapsize -= Prev->size + sizeof(chunkhead);
        }

        else if(Prev != 0 && Prev->info == 0) // if only prev is free
        {
            Prev->size = temp->size + Prev->size + sizeof(chunkhead);
            Prev->next = temp->next;
            heapsize -= Prev->size + sizeof(chunkhead);
        }

        else if(Next != 0 && Next->info == 0) // if only next is free
        {
            temp->size = temp->size + Next->size + sizeof(chunkhead);
            temp->next = Next->next;
            heapsize -= temp->size + sizeof(chunkhead);
        }
    }
    else
    {
        heapsize -= temp->size + sizeof(chunkhead);
    }

    if(heapsize == 0) // if heap has no allocated memory move program break;
    {
        brk(head);
        head = NULL;
    }

    return;
} 

void analyze() 
 { 
    printf("\n--------------------------------------------------------------\n"); 
    if(head == NULL) 
    { 
        printf("no heap\n"); 
        return; 
    } 
    chunkhead* ch = head; 
    for (int no=0; ch; ch = (chunkhead*)ch->next,no++) 
    { 
        printf("%d | current addr: %x |", no, ch); 
        printf("size: %d | ", ch->size + (sizeof(chunkhead))); 
        printf("info: %d | ", ch->info); 
        printf("next: %x | ", ch->next); 
        printf("prev: %x", ch->prev); 
        printf("      \n"); 
    } 
    printf("program break on address: %x\n",sbrk(0)); 
 } 

void main()
{

    byte *a,*b,*c; 
    a = mymalloc(4096*2); 
    b = mymalloc(1000);
    
    analyze(); 

    myfree(b);
    myfree(a);


    analyze();
    
    return;
}