#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{

    int treeHeight; // Simply the Height of the Tree

    if(argc == 1)
    {
        printf("Please enter a tree height: ");
        scanf("%d", &treeHeight);
    }
    if(argc == 2)
    {
        treeHeight = atol(argv[1]);
    }   

    for(int i = 0; i < treeHeight; i++) // Loops through tree height times
    {
        for(int j = 0; j < treeHeight - i; j++)//Prints spaces of height - ind.
        {
            printf(" ");
        }
        for(int k = 0; k < 2*i+1; k++)// Checks and prints and odd amount of *
        {
            printf("*");
        }
        printf("\n"); // Gets ready for the next set of leaves. 
    }


    for(int x = 0; x < 3; x++)
    {
        for(int j = 0; j < treeHeight; j++)
        {
            printf(" ");
        }
        printf("*\n");

    }


    return 0;
}
