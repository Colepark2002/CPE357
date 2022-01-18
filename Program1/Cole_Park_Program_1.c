#include <stdio.h>
#include <stdlib.h>

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
    DWORD biClrUsed;  //number of colors used by th ebitmap 
    DWORD biClrImportant;  //number of colors that are important 
}tagBITMAPINFOHEADER;

int main(int argc, char* argv[])
{

    

    if(argc != 5)
    {
        perror("Usage: ./[programName] [imagefile1] [imagefile2] [ratio] [outputfile]\n");
        exit(EXIT_FAILURE);
    }

    double ratio = atof(argv[3]);
    if(ratio == 0)
    {
        perror("Invalid Ratio Input\n");
        exit(EXIT_FAILURE);
    }

    FILE *bmp1 = fopen(argv[2], "rb");
    FILE *bmp2 = fopen(argv[3], "rb");
    FILE *outfile = fopen(argv[4], "wb");

    if(bmp1 == NULL|| bmp2 == NULL || outfile == NULL)
    {
        perror("Error Opening file\n");
        exit(EXIT_FAILURE);
    }
    
    tagBITMAPFILEHEADER *bmp1FileHeader = (tagBITMAPFILEHEADER*)malloc(sizeof(tagBITMAPFILEHEADER));
    tagBITMAPFILEHEADER *bmp2FileHeader = (tagBITMAPFILEHEADER*)malloc(sizeof(tagBITMAPFILEHEADER));

    tagBITMAPINFOHEADER *bmp1InfoHeader = (tagBITMAPINFOHEADER*)malloc(sizeof(tagBITMAPINFOHEADER));
    tagBITMAPINFOHEADER *bmp2InfoHeader = (tagBITMAPINFOHEADER*)malloc(sizeof(tagBITMAPINFOHEADER));

    if(bmp1FileHeader == NULL|| bmp1InfoHeader == NULL || bmp2FileHeader == NULL || bmp2InfoHeader)
    {
        perror("Malloc Error\n");
        exit(EXIT_FAILURE);
    }

    byte bmp1ColorInfo;
    byte bmp2ColorInfo;

    fread(bmp1FileHeader,sizeof(byte),14,bmp1);
    fread(bmp1InfoHeader,sizeof(byte),40,bmp1);

    fread(bmp2FileHeader,sizeof(byte),14,bmp2);
    fread(bmp2InfoHeader,sizeof(byte),40,bmp2);
    
    // MAKE MASSIVE FOR LOOP TO LOOP THROUGH PIXELS YAYYYYYYYYYYYY
    

    return;
}