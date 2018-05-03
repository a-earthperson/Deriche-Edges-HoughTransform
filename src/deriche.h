//
// Created by Arjun on 2/23/17.
//

#ifndef HOUGHTRANSFORM_DERICHE_H
#define HOUGHTRANSFORM_DERICHE_H

#include "mat.h"
#include "hough.h"

#define MAGNITUDE_LIMIT 255.0

typedef struct {
    float a1;
    float a2;
    float a3;
    float a4;
    float a5;
    float a6;
    float a7;
    float a8;
    float b1;
    float b2;
    float c1;
    float c2;
} Coeffs;

typedef struct {
    Coeffs *blur;
    Coeffs *xGradient;
    Coeffs *yGradient;
} DericheCoeffs;


/**
 * @brief Applies the Deriche Filter to the source image by running the following passes: L-to-R horizontal, R-to-L
 * horizontal, T-to-B vertical, B-to-T vertical. Output is stored inplace in the source matrix.
 * @param image Source Mat, Output Mat
 * @param coeffs Deriche coefficients pre-generated with known alpha
 */
void applyDericheFilter(Mat *image, const Coeffs *coeffs);

void hysteresisConnect(Mat *image, size_t x, size_t y, int low);

unsigned int hysteresisThreshold(Mat *image, int low, int high);

float fillBlurCoeffs(Coeffs *blurCoeffs, float ALPHA);

DericheCoeffs *DericheCoeffs_generate(float ALPHA);

void DericheCoeffs_destroy(DericheCoeffs *dericheCoeffs);

void calculateGradientIntensities(Mat *xGrad, Mat* yGrad);

void performMagnitudeSupression(Mat *xGrad, Mat* yGrad);

void writeCoeffs(Coeffs *coeffs, char *name);

void writeDericheCoeffs(DericheCoeffs *coeffs);
#endif //HOUGHTRANSFORM_DERICHE_H