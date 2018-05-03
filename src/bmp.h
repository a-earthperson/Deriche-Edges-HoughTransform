#ifndef BMP
#define BMP

#include <stdio.h>
#include <stdlib.h>

#pragma pack(1)
typedef struct BitMap
{
    unsigned short int Type;
    unsigned int Size;
    unsigned short int Reserved1, Reserved2;
    unsigned int Offset;
} Header;

typedef struct BitMapInfo
{
    unsigned int Size;
    int Width, Height;
    unsigned short int Planes;
    unsigned short int Bits;
    unsigned int Compression;
    unsigned int ImageSize;
    int xRes, yRes;
    unsigned int Colors;
    unsigned int ImptColors;
} InfoHeader;

typedef struct BitMapDetails
{
    Header *header;
    InfoHeader *infoHeader;
    unsigned char *bitmap;
} BitMapHeader;


BitMapHeader *imread(char* name);

void BitMapHeader_destroy(BitMapHeader *bmp);

void imshow(char* name,unsigned char* bitmap);

#endif