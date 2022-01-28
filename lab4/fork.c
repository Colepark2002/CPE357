#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>


typedef unsigned char byte;
typedef unsigned short WORD; 
typedef unsigned int DWORD; 
typedef unsigned int LONG;

typedef struct tagBITMAPFILEHEADER 
{ 
    WORD bfType;  //specifies the file type 
    DWORD bfSize;  //specifies the size in bytes of the bitmap file 
    WORD bfReserved1;  //reserved; must be 0 
    WORD bfReserved2;  //reserved; must be 0 
    DWORD bfOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits 
}tagBITMAPFILEHEADER; 

typedef struct tagBITMAPINFOHEADER 
{ 
    DWORD biSize;  //specifies the number of bytes required by the struct 
    LONG biWidth;  //specifies width in pixels 
    LONG biHeight;  //species height in pixels 
    WORD biPlanes; //specifies the number of color planes, must be 1 
    WORD biBitCount; //specifies the number of bit per pixel 
    DWORD biCompression;//specifies the type of compression 
    DWORD biSizeImage;  //size of image in bytes 
    LONG biXPelsPerMeter;  //number of pixels per meter in x axis 
    LONG biYPelsPerMeter;  //number of pixels per meter in y axis 
    DWORD biClrUsed;  //number of colors used by the bitmap 
    DWORD biClrImportant;  //number of colors that are important 
}tagBITMAPINFOHEADER;

int main(int argc, char* argv[])
{

    

    if(argc != 5) // incorrect input check.
    {
        perror("Usage: ./[programName] [IMAGEFILE] [BRIGHTNESS] [PARALLEL] [OUTPUTFILE]\n[IMAGEFILE] : your bit map image\n[BRIGHTNESS]: a value between 0 and 1\n[PARALLEL]: 0 for working without fork() and 1 with fork()\n[OUTPUTFILE]: the output file\n");
        exit(EXIT_FAILURE);
    }

    char *str = "0";
    double brightness = atof(argv[3]);
    if((str != argv[2] && brightness == 0) || brightness < 0 || brightness > 1)
    {
        perror("Invalid Brightness Input\n");
        exit(EXIT_FAILURE);
    }

    int parrallel = atoi(argv[3]);
    
    if((str != argv[3] && parrallel == 0) || (parrallel != 0 || parrallel != 1))
    {
        perror("Invalid Parrallel Input\n");
        exit(EXIT_FAILURE);
    }

    FILE *bmp = fopen(argv[1], "rb");
    FILE *outfile = fopen(argv[4], "wb");

    if(bmp == NULL||outfile == NULL) // checks for file errors.
    {
        perror("Error Opening file\n");
        exit(EXIT_FAILURE);
    }
    
    tagBITMAPFILEHEADER *bmpFileHeader = (tagBITMAPFILEHEADER*)mmap(NULL, sizeof(tagBITMAPFILEHEADER), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1,0);

    tagBITMAPINFOHEADER *bmpInfoHeader = (tagBITMAPINFOHEADER*)mmap(NULL, sizeof(tagBITMAPINFOHEADER), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1,0);

    if(bmpFileHeader == NULL) // check for malloc errors.
    {
        perror("Malloc Error for bmpFileHeader\n");
        exit(EXIT_FAILURE);
    }
    if(bmpInfoHeader == NULL)
    {
        perror("Malloc Error for bmpInfoHeader\n");
        exit(EXIT_FAILURE);
    }

    byte bmpColorInfo;
    byte resultColor;
    byte overflow = 255;

    fread(bmpFileHeader,sizeof(byte),14,bmp); // reads in the headers for first file.
    fread(bmpInfoHeader,sizeof(byte),40,bmp);


    int paddedBytes = (bmpInfoHeader->biWidth * 3) % 4;
    byte padding = 0;

    fwrite(bmpFileHeader, sizeof(byte), 14, outfile); // writes the file header.
    fwrite(bmpInfoHeader, sizeof(byte), 40, outfile); // writes the info header.

    

    if(parrallel == 0)
    {
        for(int y = 0; y < bmpInfoHeader->biHeight; y++) // loops through pixel height amount of times.
        {
            for(int x = 0; x < ((bmpInfoHeader->biWidth * 3) - paddedBytes); x++) 
            {
                fread(&bmpColorInfo, sizeof(byte), 1, bmp); // reads a pixel byte information, the color type is irrelevant.
                
                resultColor = bmpColorInfo + (bmpColorInfo * brightness); // brightens the color
                
                if (resultColor > 255)
                    fwrite(&overflow, sizeof(byte), 1, outfile);
                else 
                    fwrite(&resultColor, sizeof(byte), 1, outfile); // writes new color to the file.
            }
            
            fread(NULL, sizeof(byte), paddedBytes, bmp); // skips the padded bytes.
            
            for(int i = 0; i < paddedBytes; i++) // writes the padded bytes to the outfile.
            {
                fwrite(&padding, sizeof(byte), 1, outfile);
            }
        }
    }

    else
    {
        int childHeight, parentHeight;
        pid_t pid = fork();
        if((bmpInfoHeader->biHeight) % 2 != 0)
        {
            parentHeight = (bmpInfoHeader->biHeight)/2 + 1;
            childHeight = (bmpInfoHeader->biHeight)/2;
        }
        else
        {
            parentHeight = (bmpInfoHeader->biHeight)/2;
            childHeight = (bmpInfoHeader->biHeight)/2;
        }
        
        
        if(pid == 0) // if child
        {
            fseek(bmp,bmpInfoHeader->biHeight * bmpInfoHeader->biWidth, SEEK_CUR); // moves the file pointer ahead to write simultaneously to the file with parent
            
            for(int y = 0; y < childHeight; y++) // loops through child height amount of times.
            {
                for(int x = 0; x < ((bmpInfoHeader->biWidth * 3) - paddedBytes); x++) 
                {
                    fread(&bmpColorInfo, sizeof(byte), 1, bmp); // reads a pixel byte information, the color type is irrelevant.
                    
                    resultColor = bmpColorInfo + (bmpColorInfo * brightness); // brightens the color
                    
                    if (resultColor > 255)
                        fwrite(&overflow, sizeof(byte), 1, outfile);
                    else 
                        fwrite(&resultColor, sizeof(byte), 1, outfile); // writes new color to the file.
                }
                
                fread(NULL, sizeof(byte), paddedBytes, bmp); // skips the padded bytes.
                
                for(int i = 0; i < paddedBytes; i++) // writes the padded bytes to the outfile.
                {
                    fwrite(&padding, sizeof(byte), 1, outfile);
                }   
            }
        }

        if(pid > 0) // if parent;
        {
            for(int y = 0; y < parentHeight; y++) // loops through parent height amount of times.
            {
                for(int x = 0; x < ((bmpInfoHeader->biWidth * 3) - paddedBytes); x++) 
                {
                    fread(&bmpColorInfo, sizeof(byte), 1, bmp); // reads a pixel byte information, the color type is irrelevant.
                    
                    resultColor = bmpColorInfo + (bmpColorInfo * brightness); // brightens the color
                    
                    if (resultColor > 255)
                        fwrite(&overflow, sizeof(byte), 1, outfile);
                    else 
                        fwrite(&resultColor, sizeof(byte), 1, outfile); // writes new color to the file.
                }
                
                fread(NULL, sizeof(byte), paddedBytes, bmp); // skips the padded bytes.
                
                for(int i = 0; i < paddedBytes; i++) // writes the padded bytes to the outfile.
                {
                    fwrite(&padding, sizeof(byte), 1, outfile);
                }
            }
            wait(NULL);
        }
    }
    
    
    munmap(bmpFileHeader, sizeof(tagBITMAPFILEHEADER)); // unmap final information.
    munmap(bmpInfoHeader, sizeof(tagBITMAPINFOHEADER));

    fclose(bmp); // closing my files.
    fclose(outfile);
    
    return 0;
}