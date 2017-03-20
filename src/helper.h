//
// Created by Work Account on 2/23/17.
//

#ifndef HOUGHTRANSFORM_HELPER_H
#define HOUGHTRANSFORM_HELPER_H

#include "bmp.h"

typedef struct
{
    size_t width;
    size_t height;
    double *data;
} Mat;

typedef struct
{
    size_t width;
    size_t height;
    double *mag;
    double *dir;
} GradMat;

typedef struct
{
    double ALPHA_BLUR;
    double ALPHA_GRADIENT;
    double HYSTERESIS_THRESHOLD_LOW;
    double HYSTERESIS_THRESHOLD_HIGH;
} Options;


/**
 * Writes the @typedef Mat datatype to CSV
 * @param name
 * @param imageMap
 */
void writeMat_toCSV(char* name, Mat imageMap) {

    FILE *MATout = fopen(name, "w");

    if(MATout == NULL)
    {
        perror("Unable to write file, exiting");
        return;
    }

    const size_t w = imageMap.width;
    const size_t h = imageMap.height;

    size_t x, y;

    for(y = 0; y < h; y++)
    {
        for(x = 0; x < (w-1); x++)
        {
            fprintf(MATout, "%f,", imageMap.data[(y*w) + x]);
        }
        fprintf(MATout, "%f\n", imageMap.data[(y * (w)) + (w-1)]);
    }

}

/**
 * Allocates memory for writing to a @typedef GradMat.
 * @param width
 * @param height
 * @return
 */
GradMat createGradMatrix(const unsigned int width, const unsigned int height) {

    GradMat newMatrix;
    newMatrix.width  = width;
    newMatrix.height = height;
    const unsigned int pixel_count = width * height;

    newMatrix.mag = malloc(pixel_count * sizeof(double));
    newMatrix.dir = malloc(pixel_count * sizeof(double));

    if (newMatrix.mag == NULL || newMatrix.dir == NULL)
    {
        printf("memory asked :%lu \n", pixel_count * sizeof(double));
        perror("unable to allocate memory for matrix. exiting.");
        exit(1);
    }

    return newMatrix;
}

/**
 * Allocates memory for writing to a @typedef Mat. Memory can be zeroed out based on needs.
 * @param width
 * @param height
 * @param zeroed
 * @return
 */
Mat createMatrix(const size_t width, const size_t height, const unsigned int zeroed) {

    Mat newMatrix;
    newMatrix.width = width;
    newMatrix.height = height;

    const size_t pixel_count = width * height;

    newMatrix.data = malloc(pixel_count * sizeof(double));

    if (newMatrix.data == NULL)
    {
        printf("memory asked :%lu", pixel_count * sizeof(double));
        perror("unable to allocate memory for matrix. exiting.");
        exit(1);
    }

    /* Set matrix values to zero */
    if(!zeroed)
    {
        size_t i;
        for (i = 0; i < pixel_count; i++)
        {
            newMatrix.data[i] = 0.0;
        }
    }

    return newMatrix;
}

/**
 * Free all memory associated with the @typedef Mat object
 * @param toDestroy
 */
void destroyMatrix(Mat *toDestroy) {
    free(toDestroy->data);
}

/**
 * Free all memory associated with the @typedef GradMat object
 * @param toDestroy
 */
void destroyGradMatrix(GradMat *toDestroy) {
    free(toDestroy->mag);
    free(toDestroy->dir);
}
/**
 * Reads in a BMP file and outputs a @typedef Mat datatype, normalizing the matrix magnitudes if option is enabled
 * @param image_data
 * @return
 */
Mat color2gray(unsigned char *image_data) {

    Mat grayImage = createMatrix((size_t) InfoHeader.Width, (size_t) InfoHeader.Height, 0);
    const size_t pixel_count = grayImage.width * grayImage.height;
    double most_bright_pixel = 0.0;

    /* calculate brightness for all pixels & find the brightest pixel */
    size_t i;
    for(i = 0; i < pixel_count; i++)
    {
        const size_t idx = 3*i;
        const double R = (double)((unsigned char) image_data[idx]);     // 3i + 0
        const double G = (double)((unsigned char) image_data[idx+1]); // 3i + 1
        const double B = (double)((unsigned char) image_data[idx+2]); // 3i + 2

        /* store image magnitude in gray image mat */
        grayImage.data[i] = hypot(R, hypot(G, B));

        /* find the brightest pixel */
        most_bright_pixel = (grayImage.data[i] > most_bright_pixel) ? grayImage.data[i] : most_bright_pixel;
    }

    /** calculate normalization factor alpha **/
    if(NORMALIZE_ALPHA)
    {
        const double ALPHA = (MAX_BRIGHTNESS / most_bright_pixel);
        for (i = 0; i < pixel_count; i++)
        {
            grayImage.data[i] = MAX_BRIGHTNESS - (grayImage.data[i] * ALPHA);
            if(VERBOSE) printf("gray:%f\n", grayImage.data[i]);
        }
    }

    return grayImage;
}

/**
 * Performs the same work as @fn color2gray, but ensures that BMP related memory is freed once the matrix has been created
 * @param name
 * @return
 */
Mat imreadGray(char* name) {
    unsigned char *BMPin = imread(name);
    Mat grayMatrix = color2gray(BMPin);
    m_free(BMPin);
    return grayMatrix;
}
#endif //HOUGHTRANSFORM_HELPER_H
