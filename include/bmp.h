#ifndef BMP
#define BMP

#include <stdio.h>
#include <stdlib.h>

#pragma pack(1)
struct BitMap
{
    unsigned short int Type;
    unsigned int Size;
    unsigned short int Reserved1, Reserved2;
    unsigned int Offset;
} Header;

struct BitMapInfo
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


unsigned char* imread(char* name);

void imshow(char* name,unsigned char* bitmap);

#endif