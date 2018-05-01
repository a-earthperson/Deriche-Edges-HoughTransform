//
// Created by Arjun on 2/23/17.
//

#ifndef HOUGHTRANSFORM_HELPER_H
#define HOUGHTRANSFORM_HELPER_H

#define MAX_BRIGHTNESS 255.0f

#include "bmp.h"
#include <math.h>
#include <stdlib.h>
#include <float.h>

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

typedef float (*elementwise)(float, float);

/**
 * Writes the @typedef Mat datatype to CSV
 * @param name
 * @param imageMap
 */
void Mat2CSV(char* name, Mat *imageMap, char* ext);

/**
 * Allocates memory for writing to a @typedef Mat. Memory can be zeroed out based on needs.
 * @param width
 * @param height
 * @param zeroed
 * @return
 */
Mat *Mat_generate(size_t width, size_t height, unsigned int zeroed);

Mat *Mat_copy(Mat *src);

float multipy(float x, float y);

void Mat_elementwise(Mat* x, Mat*y, float elementwise (float, float));

/**
 * Free all memory associated with the @typedef Mat object
 * @param toDestroy
 */
void Mat_destroy(Mat *toDestroy);

float normalizeImageWithMax(Mat *image, float max, unsigned char inverted);

float normalizeImage(Mat *image);

/**
 * Reads in a BMP file and outputs a @typedef Mat datatype, normalizing the matrix magnitudes if option is enabled
 * @param image_data
 * @return
 */
Mat *color2gray(unsigned char *image_data);

/**
 * Performs the same work as @fn color2gray, but ensures that BMP related memory is freed once the matrix has been created
 * @param name
 * @return
 */
Mat *imreadGray(char* name);

#endif //HOUGHTRANSFORM_HELPER_H
