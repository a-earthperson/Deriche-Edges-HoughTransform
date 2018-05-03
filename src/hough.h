//
// Created by Arjun on 2/20/17.
//

#ifndef HOUGHTRANSFORM_HOUGH_H
#define HOUGHTRANSFORM_HOUGH_H

#define KERNEL_MULTIPLIER 20
#define DEG2RAD (M_PI / 180.0f)

#include <math.h>
#include "bmp.h"
#include "mat.h"
#include "deriche.h"

Mat *HoughTransform(const Mat *grayscale_img);

double getPolygonOrientation(Mat *image);

void getPolygonSideLengths(Mat *image, double *out_array);

void suppressNeighborsHough(Mat *image, size_t x, size_t y);

#endif //HOUGHTRANSFORM_HOUGH_H