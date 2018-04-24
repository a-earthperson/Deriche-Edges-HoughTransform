//
// Created by Arjun on 2/23/17.
//

#ifndef HOUGHTRANSFORM_HELPER_H
#define HOUGHTRANSFORM_HELPER_H

#define MAX_BRIGHTNESS 255.0f

#include "bmp.h"
#include <math.h>
#include <stdlib.h>

typedef struct
{
    size_t width;
    size_t height;
    float *data;
} Mat;

typedef struct
{
    float ALPHA_BLUR;
    float ALPHA_GRADIENT;
    unsigned int HYSTERESIS_THRESHOLD_LOW;
    unsigned int HYSTERESIS_THRESHOLD_HIGH;
} Options;


/**
 * Writes the @typedef Mat datatype to CSV
 * @param name
 * @param imageMap
 */
void Mat2CSV(char* name, Mat *imageMap, char* ext) {

    char buffer[1024];
    sprintf(buffer, "%s%s", name,  ext);

    FILE *MATout = fopen(buffer, "w+");

    if(MATout == NULL)
    {
        perror("Unable to write file, exiting");
        return;
    }

    const size_t w = imageMap->width;
    const size_t h = imageMap->height;

    size_t x, y;

    for(y = 0; y < h; y++)
    {
        for(x = 0; x < (w-1); x++)
        {
            fprintf(MATout, "%.1f,", imageMap->data[(y*w) + x]);
        }
        fprintf(MATout, "%.1f\n", imageMap->data[(y * (w)) + (w-1)]);
    }
}

/**
 * Allocates memory for writing to a @typedef Mat. Memory can be zeroed out based on needs.
 * @param width
 * @param height
 * @param zeroed
 * @return
 */
Mat *Mat_generate(const size_t width, const size_t height, const unsigned int zeroed) {

    if(width < 1 || height < 1)
        exit(1);

    const size_t pixel_count = width * height;

    Mat *newMatrix = (Mat *) malloc(sizeof(Mat));
    float *data = (zeroed) ? calloc(pixel_count, sizeof(float)): malloc(pixel_count * sizeof(float));

    if (data == NULL || newMatrix == NULL)
    {
        printf("memory asked :%lu\n", pixel_count * sizeof(float));
        perror("unable to allocate memory for matrix. exiting.");
        exit(1);
    }

    if(!zeroed)
    {
        size_t i;
        for(i = 0; i < pixel_count; i++)
            data[i] = 0.0f;
    }

    if(VERBOSE)
    {
        printf("Bytes assigned for Mat(w:%lu  x h: %lu): %lu\n", width, height, pixel_count * sizeof(float));
    }

    newMatrix->width = width;
    newMatrix->height = height;
    newMatrix->data = data;
    return newMatrix;
}

Mat *Mat_copy(Mat *src) {

    Mat *copy = Mat_generate(src->width, src->height, 0);

    const size_t count = src->width * src->height;

    size_t i;

    // copy the data values
    for(i = 0; i < count; i++)
    {
        copy->data[i] = src->data[i];
    }

    return copy;
}

/**
 * Free all memory associated with the @typedef Mat object
 * @param toDestroy
 */
void Mat_destroy(Mat *toDestroy) {
    free(toDestroy->data);
    free(toDestroy);
}

void normalizeImageWithMax(Mat *image, const float max, const unsigned char inverted) {

    /* get image size parameters */
    const size_t img_width = image->width;
    const size_t img_height = image->height;
    const size_t pixel_count = img_height * img_width;

    if(max == 0)
    {
        return;
    }

    const float ALPHA = (MAX_BRIGHTNESS / max);

    size_t i;

    // if invert colors set to true
    if(inverted)
    {
        for (i = 0; i < pixel_count; i++)
        {
            image->data[i] = MAX_BRIGHTNESS - (image->data[i] * ALPHA);
        }
    }

    else
    {
        for (i = 0; i < pixel_count; i++)
        {
            image->data[i] = (image->data[i] * ALPHA);
        }
    }
}

void normalizeImage(Mat *image) {

    /* get image size parameters */
    const size_t img_width = image->width;
    const size_t img_height = image->height;
    const size_t pixel_count = img_height * img_width;

    float max = 0;

    /* calculate brightness for all pixels & find the brightest pixel */
    size_t i;
    for (i = 0; i < pixel_count; i++)
    {
        if (image->data[i] > max)
        {
            max = image->data[i];
        }
    }

    normalizeImageWithMax(image, max, 0);
}

/**
 * Reads in a BMP file and outputs a @typedef Mat datatype, normalizing the matrix magnitudes if option is enabled
 * @param image_data
 * @return
 */
Mat *color2gray(unsigned char *image_data) {

    Mat *grayImage = Mat_generate((size_t) InfoHeader.Width, (size_t) InfoHeader.Height, 0);
    const size_t pixel_count = grayImage->width * grayImage->height;

    /* calculate brightness for all pixels & find the brightest pixel */
    size_t i; float max = 0;

    for(i = 0; i < pixel_count; i++)
    {
        const size_t idx = 3*i;
        const float R = (float)((unsigned char) image_data[idx]);     // 3i + 0
        const float G = (float)((unsigned char) image_data[idx+1]);   // 3i + 1
        const float B = (float)((unsigned char) image_data[idx+2]);   // 3i + 2
        const float V = hypotf(R, hypotf(G, B));
        if(V > max)
        {
            max = V;
        }
        grayImage->data[i] = V;
    }

    normalizeImageWithMax(grayImage, max, 1);

    return grayImage;
}

/**
 * Performs the same work as @fn color2gray, but ensures that BMP related memory is freed once the matrix has been created
 * @param name
 * @return
 */
Mat *imreadGray(char* name) {
    unsigned char *BMPin = imread(name);
    Mat *grayMatrix = color2gray(BMPin);
    free(BMPin);
    return grayMatrix;
}
#endif //HOUGHTRANSFORM_HELPER_H
