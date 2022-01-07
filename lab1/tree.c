#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{

    int treeHeight; // Simply the Height of the Tree
    FILE* outfile;

    if(argc == 1)
    {
        printf("Please enter a tree height: ");
        scanf("%d", &treeHeight);
        printf("\n");
    }
    else if(argc == 2)
    {
        treeHeight = atol(argv[1]);
        if(atol(argv[1]) == 0)
        {
            perror("Invalid Tree Height Please enter an Integer");
            exit(EXIT_FAILURE);
        }
    }  

    else if(argc == 3)
    {
        treeHeight = atol(argv[1]);
        outfile = fopen(argv[2], "w");
        if (outfile == NULL)
        {
            perror("Error Opening File\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        perror("Invalid number of Arguments");
        exit(EXIT_FAILURE);
    }

    if(treeHeight < 4)
    {
        perror("Tree Height is too small.");
        exit(EXIT_FAILURE);
    }
    if(treeHeight > 15)
    {
        perror("Tree Height is too large.");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < (treeHeight - 3); i++) // Loops through tree height times
    {
        for(int j = 0; j < treeHeight - i; j++)//Prints spaces of height - ind.
        {
            printf(" ");
            if(argc == 3)
                fprintf(outfile, " ");
        }
        for(int k = 0; k < 2*i+1; k++)// Checks and prints and odd amount of *
        {
            printf("*");
            if(argc == 3)
                fprintf(outfile, "*");
        }
        printf("\n"); // Gets ready for the next set of leaves.
        if(argc == 3)
                fprintf(outfile, "\n"); 
    }


    for(int x = 0; x < 3; x++)
    {
        for(int j = 0; j < treeHeight; j++)
        {
            printf(" ");
            if(argc == 3)
                fprintf(outfile, " ");
        }
        printf("*\n");
        if(argc == 3)
                fprintf(outfile, "*\n");

    }

    if(argc == 3)
    {
        fclose(outfile);
    }

    return 0;
}
