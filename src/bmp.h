//#ifndef BMP
//#define BMP
//#include "m_mem.h"
//
//#pragma pack(1)
//typedef struct
//{
//   unsigned short int Type;
//   unsigned int Size;
//   unsigned short int Reserved1, Reserved2;
//   unsigned int Offset;
//} BitMapHeader;
//
//typedef struct
//{
//   unsigned int Size;
//   int Width, Height;
//   unsigned short int Planes;
//   unsigned short int Bits;
//   unsigned int Compression;
//   unsigned int ImageSize;
//   int xRes, yRes;
//   unsigned int Colors;
//   unsigned int ImptColors;
//} BitMapInfoHeader;
//
//typedef struct
//{
//    unsigned char *Values;
//    size_t Length;
//} BitMapData;
//
//typedef struct
//{
//    BitMapHeader Header;
//    BitMapInfoHeader InfoHeader;
//    BitMapData Data;
//} BitMap;
//
//BitMap imread(char* name)
//{
//    BitMap bitmap;
//    FILE *BMP_in = fopen (name, "rb");
//    int i, datasize;
//    int padding;
//    unsigned char *pad;
//
//	if (BMP_in == NULL)
//	{
//        printf("\nCannot open file\n");
//        exit(1);
//	}
//
//	printf("Reading the bmp header...\n");
//	fread(&(bitmap.Header), sizeof(BitMapHeader), 1, BMP_in);
//	fread(&(bitmap.InfoHeader), sizeof(BitMapInfoHeader), 1, BMP_in);
//
//    bitmap.InfoHeader.Height = abs(bitmap.InfoHeader.Height);
//    bitmap.InfoHeader.Width  = abs(bitmap.InfoHeader.Width);
//
//	printf("size of Header = %lu\n", sizeof(BitMapHeader));
//	printf("size of InfoHeader = %lu\n", sizeof(BitMapInfoHeader));
//	printf("Width: %d\n", (bitmap.InfoHeader.Width));
//	printf("Height: %d\n", (bitmap.InfoHeader.Height));
//
//	bitmap.Data.Length = bitmap.InfoHeader.Width*bitmap.InfoHeader.Height*3;
//
//	padding = (4 - (abs(bitmap.InfoHeader.Width*3) % 4)) % 4 ;
//	pad = malloc(padding*sizeof(unsigned char));
//
//	bitmap.Data.Values = m_malloc(bitmap.Data.Length);
//
//
//    if (!bitmap.Data.Values)
//     {
//    	 printf("out of memory!\n");
//     }
//     else
//     {
//    	 printf("Successfully allocated memory for the bitmap\n");
//     }
//
//    unsigned char* data_values = bitmap.Data.Values;
//    printf("Reading BMP file...\n");
//    for (i=0; i < bitmap.Data.Length; i++)
//    {
//         fread(&data_values[i], 1,1,  BMP_in);
//
//       	 if (i % (3*bitmap.InfoHeader.Width) == 3*bitmap.InfoHeader.Width -1)
//       	 {
//       		fread(&pad, padding*sizeof(unsigned char),1,  BMP_in);
//       	 }
//    }
//
//    fclose(BMP_in);
//    free(pad);
//
//    printf("Finish reading BMP file...\n");
//
//	return bitmap;
//}
//
//void imshow(char* name, BitMap bitmap)
//{
//
//    FILE *BMP_out;
//    int filesize, datasize, i;
//	int padding;
//	unsigned char *pad;
//
//    datasize = bitmap.InfoHeader.Width*bitmap.InfoHeader.Height*3;
//    filesize = 54 + 3*bitmap.InfoHeader.Width*bitmap.InfoHeader.Height;
//
//	padding = (4 - ((bitmap.InfoHeader.Width*3) % 4)) % 4 ;
//	pad = malloc(padding*sizeof(unsigned char));
//
//	for (i = 0; i < padding; i++)
//	{
//		pad[i] = 0;
//	}
//
//
//    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
//    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
//
//    bmpfileheader[ 2] = (unsigned char)(filesize    );
//    bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
//    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
//    bmpfileheader[ 5] = (unsigned char)(filesize>>24);
//    bmpinfoheader[ 4] = (unsigned char)(       bitmap.InfoHeader.Width    );
//    bmpinfoheader[ 5] = (unsigned char)(       bitmap.InfoHeader.Width>> 8);
//    bmpinfoheader[ 6] = (unsigned char)(       bitmap.InfoHeader.Width>>16);
//    bmpinfoheader[ 7] = (unsigned char)(       bitmap.InfoHeader.Width>>24);
//    bmpinfoheader[ 8] = (unsigned char)(       bitmap.InfoHeader.Height    );
//    bmpinfoheader[ 9] = (unsigned char)(       bitmap.InfoHeader.Height>> 8);
//    bmpinfoheader[10] = (unsigned char)(       bitmap.InfoHeader.Height>>16);
//    bmpinfoheader[11] = (unsigned char)(       bitmap.InfoHeader.Height>>24);
//
//    BMP_out = fopen(name,"wb");
//    fwrite(bmpfileheader,1,14,BMP_out);
//    fwrite(bmpinfoheader,1,40,BMP_out);
//
//    printf("Writing BMP file...\n");
//
//    unsigned char* data_values = bitmap.Data.Values;
//    for(i=0; i < datasize; i++)
//    {
//
//    	if (i%(bitmap.InfoHeader.Width*3) == 0)
//    	{
//    		printf("i = %d\n",i);
//    	}
//
//        fwrite(&data_values[i], 1,1,  BMP_out);
//
//        if (i % (3*bitmap.InfoHeader.Width) == 3*bitmap.InfoHeader.Width -1)
//        {
//        	fwrite(&pad, padding*sizeof(unsigned char),1,  BMP_out);
//        }
//
//     }
//    printf("Finish writing BMP file...\n");
//    fclose(BMP_out);
//    free(pad);
//}
//
//#endif
