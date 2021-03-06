#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

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
    int normal = 0;
    int padding;
    
    FILE* outfile = fopen("decompressed2.bmp", "wb");
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
    //chunk chunkData[(totalbytes/3)];
    chunk *chunkData = mmap(NULL, (sizeof(chunk) * (totalbytes/3)), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *bitread = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *bitread = 0;
    fread(&colors,sizeof(byte),header.palettecolors * 12,infile);
    int chunkI = 0;
    for(int y = 0; y < (totalbytes/3); y++)
    {
        fread(&(chunkData[y].color_index),sizeof(byte),1,infile);
        fread(&(chunkData[y].count),sizeof(byte),2,infile);
    }
    fclose(infile);


    bmpFileHeader.bfType = 19778;
    bmpFileHeader.bfSize = 4320054;
    bmpFileHeader.bfReserved1 = 0;
    bmpFileHeader.bfReserved2 = 0;
    bmpFileHeader.bfOffBits = 0;
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
    byte *bmpdata =  mmap(NULL, (sizeof(byte) * (bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * 3)), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int bmpI = 0; // index in our final write array
    int bytesRead = 0; // used to determine when to pad
    int index = 0; //index of chunkData
    int sum = 0; // for debugging
    if(normal)
    {
        
        for(index = 0; index < (totalbytes/3);index++) //reads in all bytes in multiples of chunks
        {
            chunk currChunk = chunkData[index];
            col currColor = colors[currChunk.color_index];
            short count = currChunk.count;
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
        int w = fwrite(bmpdata,sizeof(byte), (bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * 3),outfile);
        munmap(chunkData,(sizeof(chunk) * (totalbytes/3)));
        fclose(outfile);
    }
    if(!normal)
    {
        int parentID = getpid();
        if(fork() == 0) // child1
        {
            int bmpI = 0;
            bytesRead = 0;
            int chunksToBeRead = (header.rowbyte_quarter[0])/3;
            for(index = 0; index < chunksToBeRead;index++) //reads in all bytes in multiples of chunks
            {
                chunk currChunk = chunkData[index];
                col currColor = colors[currChunk.color_index];
                short count = currChunk.count;
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
                    *bitread += 3;
                }
            }
            
        }
        else if(fork() == 0) //child 2
        {
            int bmpI = bmpInfoHeader.biSizeImage/4;
            bytesRead = 0;
            int chunksToBeRead = (header.rowbyte_quarter[1] - header.rowbyte_quarter[0])/3;
            index = header.rowbyte_quarter[0]/3;
            for(int x = 0; x < chunksToBeRead;x++) //reads in all bytes in multiples of chunks
            {
                chunk currChunk = chunkData[index];
                col currColor = colors[currChunk.color_index];
                short count = currChunk.count;
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
                    *bitread += 3;
                }
                index++;
            }
        }
        else if(fork() == 0) // child 3
        {
            int bmpI = (bmpInfoHeader.biSizeImage/4)*2;
            bytesRead = 0;
            int chunksToBeRead = (header.rowbyte_quarter[2] - header.rowbyte_quarter[1])/3;
            index = header.rowbyte_quarter[1]/3;
            for(int x  = 0; x < chunksToBeRead;x++) //reads in all bytes in multiples of chunks
            {
                chunk currChunk = chunkData[index];
                col currColor = colors[currChunk.color_index];
                short count = currChunk.count;
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
                    *bitread += 3;
                }
                index++;
            }
        }
        else if(getpid() == parentID)
        {
            int bmpI = (bmpInfoHeader.biSizeImage/4)*3;
            bytesRead = 0;
            int chunksToBeRead = (header.rowbyte_quarter[3] - header.rowbyte_quarter[2])/3;
            index = header.rowbyte_quarter[2]/3;
            for(int x = 0; x < chunksToBeRead;x++) //reads in all bytes in multiples of chunks
            {
                chunk currChunk = chunkData[index];
                col currColor = colors[currChunk.color_index];
                short count = currChunk.count;
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
                    *bitread += 3;
                }
                index++;
            }
            wait(NULL);
            int w = fwrite(bmpdata,sizeof(byte), (bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * 3),outfile);
            munmap(chunkData,(sizeof(chunk) * (totalbytes/3)));
            munmap(bmpdata, sizeof(byte) * bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * 3);
            fclose(outfile);
        }
    }
    
    return 0;
}