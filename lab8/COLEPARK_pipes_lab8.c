#include <stdlib.h>
#include <stdio.h>

typedef unsigned char byte;

typedef struct mypipe 
{ 
    byte* pipebuffer; 
    int buffersize; 
    int start_occupied; 
    int end_occupied; 
}mypipe; 


//initializes (malloc) the pipe with a size of "size" and sets start and end. 
void init(mypipe* pipe, int size)
{
    pipe->buffersize = size;
    pipe->pipebuffer = (byte*)malloc(sizeof(byte) * size);
    pipe->end_occupied = 0;
    pipe->start_occupied = 0;
    return;
}

//writes "size" bytes from buffer into the pipe, returns size 
int mywrite(mypipe *pipe, byte* buffer, int size)
{
    int i;
    
    for (i = 0; i < size; i++)
    {
        pipe->pipebuffer[((pipe->end_occupied+i)%pipe->buffersize)] = buffer[i];
    }
    pipe->end_occupied = (pipe->end_occupied + size) % pipe->buffersize;
    return size;
}
 
//reads "size" bytes from pipe into buffer, returns how much it read (max size), 0 if pipe is empty 
int myread(mypipe* pipe, byte* buffer, int size)
{
    int i;
    
    for (i = 0; i < size; i++)
    {
        if((pipe->start_occupied + i) % pipe->buffersize == pipe->end_occupied)
        {
            return i;
        }
        buffer[i] = pipe->pipebuffer[(pipe->start_occupied + i)%pipe->buffersize];
    }
    pipe->start_occupied = (pipe->start_occupied + i) % pipe->buffersize;
    return size;
}


int main()
{
    char text[100]; 
    mypipe pipeA; 
    
    init(&pipeA, 32); 
    mywrite(&pipeA, "hello world", 12); 
    mywrite(&pipeA, "it's a nice day", 16); 
    
    myread(&pipeA, text, 12); 
    printf("%s\n", text); 
    myread(&pipeA, text, 16); 
    printf("%s\n", text); 
    
    mywrite(&pipeA, "and now we test the carryover", 30); 
    myread(&pipeA, text, 30); 
    printf("%s\n", text); 
    return 0;
}