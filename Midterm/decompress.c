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
    DWORD biClrUsed;  //number of colors used by the bitmap 
    DWORD biClrImportant;  //number of colors that are important 
}tagBITMAPINFOHEADER;

typedef struct col
{
    int r, g, b; // red, green and blue, in that order.  The values will not exceed 255!
}col;

typedef struct compressedformat
{
    int width, height;         // width and height of the image, with one byte for each color, blue, green and red
    int rowbyte_quarter[4];    // for parallel algorithms! That’s the amount in bytes which exactly splits the result image after decompression into 4 equal parts!
    int palettecolors;         // how many colors does the picture have?
}compressedformat;

typedef struct chunk
{
    byte color_index; // which of the color of the palette
    short count;      // How many pixel of the same color from color_index are continuously appearing
}chunk;

int main()
{
    tagBITMAPFILEHEADER bmpFileHeader;
    tagBITMAPINFOHEADER bmpInfoHeader;
    compressedformat header;
    chunk *currChunk;
    int normal = 1;
    int padding;
    
    FILE* outfile = fopen("decompressed.bmp", "wb");
    FILE* infile = fopen("compressed.bin","rb");
    
    if (!outfile)
    {
        perror("Error Opening Outfile");
        exit(EXIT_FAILURE);
    }
    if (!infile)
    {
        perror("Error Opening Infile");
        exit(EXIT_FAILURE);
    }
    
    fread(&header, sizeof(byte), 28,infile);
    col colors[header.palettecolors];
    int totalbytes = header.rowbyte_quarter[3];
    byte chunkData[totalbytes];
    fread(&colors,sizeof(byte),header.palettecolors * 12,infile);
    fread(&chunkData,sizeof(byte),totalbytes,infile);
    bmpFileHeader.bfType = 19778;
    bmpFileHeader.bfSize = 4320054;
    bmpFileHeader.bfReserved1 = 0;
    bmpFileHeader.bfReserved2 = 0;
    bmpFileHeader.bfOffBits = 54;
    fwrite(&bmpFileHeader, sizeof(byte), 14, outfile);

    bmpInfoHeader.biSize = 40;
    bmpInfoHeader.biWidth = 1200;
    bmpInfoHeader.biHeight = 1200;
    bmpInfoHeader.biPlanes = 1;
    bmpInfoHeader.biBitCount = 24;
    bmpInfoHeader.biCompression = 0;
    bmpInfoHeader.biSizeImage = 4320000;
    bmpInfoHeader.biXPelsPerMeter= 3780;
    bmpInfoHeader.biYPelsPerMeter = 3780;
    bmpInfoHeader.biClrUsed = 0;
    bmpInfoHeader.biClrImportant = 0;
    fwrite(&bmpInfoHeader, sizeof(byte), 40, outfile);

    padding = (bmpInfoHeader.biWidth * 3) % 4;
    byte bmpdata[bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * 3];
    int bmpI = 0; // index in our final write array
    int bytesRead = 0; // used to determine when to pad
    int index = 0; //index of chunkData
    int sum = 0; // for debugging
    if(normal)
    {
        
        for(index = 0; index < totalbytes;index+=3) //reads in all bytes in multiples of chunks
        {
            byte colorInd = chunkData[index];
            col currColor = colors[colorInd];
            short count = *((short*)(chunkData + (index + 1)));
            sum += count;
            for(int i = 0; i < count;i++) //for each chunk add count many pixels of the specified color
            {
                if(bytesRead == (bmpInfoHeader.biWidth * 3))//if at the end of the width added needed padding to file
                {
                    for(int j = 0; j < padding; j++)
                    {
                        bmpdata[bmpI++] = 0;
                        bytesRead++;
                    }
                    bytesRead = 0;
                }
                bmpdata[bmpI++] = (byte)currColor.b;
                bmpdata[bmpI++] = (byte)currColor.g;
                bmpdata[bmpI++] = (byte)currColor.r;
                bytesRead += 3;
            }
        }
    }
    else
    {

    }
    int w = fwrite(bmpdata,sizeof(byte), (bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * 3),outfile);
    printf("%d\n", w);
    return 0;
}