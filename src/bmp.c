//
// Created by Arjun on 4/30/18.
//

#include "bmp.h"

BitMapHeader* imread(char* fullpath) {

    unsigned char *bitmap;
    FILE *BMP_in = fopen (fullpath, "rb");
    int i, datasize;
    int padding;
    unsigned char *pad;

    if (BMP_in == NULL)
    {
        printf("\nCannot open file\n");
        exit(1);
    }

    BitMapHeader *bmp = (BitMapHeader *) malloc(sizeof(BitMapHeader));
    bmp->infoHeader = (InfoHeader *) malloc(sizeof(InfoHeader));
    bmp->header = (Header *) malloc(sizeof(Header));

    if(bmp == NULL || bmp ->infoHeader == NULL || bmp->header == NULL)
    {
        printf("out of memory!\n");
        exit(1);
    }

    //printf("Opening file: %s\n", fullpath);
    //printf("Reading the bmp header...\n");
    fread(bmp->header, sizeof(Header), 1, BMP_in);
    fread(bmp->infoHeader, sizeof(InfoHeader), 1, BMP_in);

    /** setting height and width to abs values since this breaks on Mac OS**/
    //printf("size of Header = %lu\n", sizeof(Header));
    //printf("size of InfoHeader = %lu\n", sizeof(InfoHeader));
    //printf("Width: %d\n", (bmp->infoHeader)->Width);
    //printf("Height: %d\n", bmp->infoHeader->Height);

    bmp->infoHeader->Width = abs(bmp->infoHeader->Width);
    bmp->infoHeader->Height = abs(bmp->infoHeader->Height);

    datasize = bmp->infoHeader->Width  * bmp->infoHeader->Height * 3;

    padding = (4 - ((bmp->infoHeader->Width * 3) % 4)) % 4 ;

    pad = (unsigned char *) malloc(padding * sizeof(unsigned char));
    bitmap = (unsigned char *) malloc((size_t) datasize);

    if (bitmap == NULL || pad == NULL)
    {
        printf("out of memory!\n");
        exit(1);
        return NULL;
    }

    //printf("Successfully allocated memory for the bitmap\n");
    //printf("Reading BMP file...\n");

    for (i=0; i < datasize; i++)
    {
        fread(&bitmap[i], 1,1, BMP_in);

        if (i % (3 * bmp->infoHeader->Width) == 3 * bmp->infoHeader->Width -1)
        {
            fread(&pad, padding*sizeof(unsigned char),1,  BMP_in);
        }

    }

    free(pad);

    fclose(BMP_in);

    //printf("Finish reading BMP file...\n");

    bmp->bitmap = bitmap;
    return bmp;
}

//void imshow(char* name,unsigned char* bitmap) {
//
//    FILE *BMP_out;
//    int filesize, datasize, i;
//    int padding;
//    unsigned char *pad;
//
//    datasize = InfoHeader.Width*InfoHeader.Height*3;
//    filesize = 54 + 3*InfoHeader.Width* InfoHeader.Height;
//
//    padding = (4 - ((InfoHeader.Width*3) % 4)) % 4 ;
//    pad = calloc(padding,sizeof(unsigned char));
//
//
//    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
//    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
//
//    bmpfileheader[ 2] = (unsigned char)(filesize    );
//    bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
//    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
//    bmpfileheader[ 5] = (unsigned char)(filesize>>24);
//    bmpinfoheader[ 4] = (unsigned char)(       InfoHeader.Width    );
//    bmpinfoheader[ 5] = (unsigned char)(       InfoHeader.Width>> 8);
//    bmpinfoheader[ 6] = (unsigned char)(       InfoHeader.Width>>16);
//    bmpinfoheader[ 7] = (unsigned char)(       InfoHeader.Width>>24);
//    bmpinfoheader[ 8] = (unsigned char)(       InfoHeader.Height    );
//    bmpinfoheader[ 9] = (unsigned char)(       InfoHeader.Height>> 8);
//    bmpinfoheader[10] = (unsigned char)(       InfoHeader.Height>>16);
//    bmpinfoheader[11] = (unsigned char)(       InfoHeader.Height>>24);
//
//    BMP_out = fopen(name,"wb");
//    fwrite(bmpfileheader,1,14,BMP_out);
//    fwrite(bmpinfoheader,1,40,BMP_out);
//
//    printf("Writing BMP file...\n");
//
//    for(i=0; i < datasize; i++)
//    {
//
//        if (i%(InfoHeader.Width*3) == 0)
//        {
//            printf("i = %d\n",i);
//        }
//
//        fwrite(&bitmap[i], 1,1,  BMP_out);
//
//        if (i % (3*InfoHeader.Width) == 3*InfoHeader.Width -1)
//        {
//            fwrite(&pad, padding*sizeof(unsigned char),1,  BMP_out);
//        }
//
//    }
//    printf("Finish writing BMP file...\n");
//    fclose(BMP_out);
//
//}

void BitMapHeader_destroy(BitMapHeader *bmp) {
    if(bmp == NULL)
        return;

    if(bmp->header != NULL)
        free(bmp->header);

    if(bmp->infoHeader != NULL)
        free(bmp->infoHeader);

    if(bmp->bitmap != NULL)
        free(bmp->bitmap);
}