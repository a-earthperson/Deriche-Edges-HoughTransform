//
// Created by Arjun on 2/20/17.
//

#ifndef HOUGHTRANSFORM_HOUGH_H
#define HOUGHTRANSFORM_HOUGH_H

#define MAX_BRIGHTNESS 255.0
#define MIN_BRIGHTNESS 0.0

#define HOUGH_THRESHOLD 0
#define NORMALIZE_ALPHA 1
#define KERNEL_MULTIPLIER 20
#define DEG2RAD (M_PI / 180.0f)

#include <math.h>
#include "bmp.h"
#include "helper.h"
#include "deriche.h"

void normalizeImage(Mat *image) {

    /* get image size parameters */
    const size_t img_width = image->width;
    const size_t img_height = image->height;
    const size_t pixel_count = img_height * img_width;

    double max = 0.0;

    /* calculate brightness for all pixels & find the brightest pixel */
    size_t i;
    for (i = 0; i < pixel_count; i++)
    {
        max = (image->data[i] > max) ? image->data[i] : max;
    }

    const double ALPHA = (MAX_BRIGHTNESS / max);

    for (i = 0; i < pixel_count; i++)
    {
        image->data[i] = (image->data[i] * ALPHA);
    }
}

Mat HoughTransform(Mat *grayscale_img) {

    size_t i;

    /** get image size parameters */
    const size_t img_width = grayscale_img->width;
    const size_t img_height = grayscale_img->height;
    const size_t img_pixel_count = img_height * img_width;

    /** calculate diagonal length for hough-transformed image **/
    const size_t diag_len = (size_t) hypot(img_height, img_width);

    /** initialize output matrix with parameters **/
    const size_t width = 180;
    const size_t height = 2 * diag_len;
    printf("hough image length:%lu, %lu\n", height, width);

    /** create matrix **/
    Mat houghMatrix = createMatrix(width, height, 1);

    /** Read read input image pixel values, and perform hough transform on each value that exceeds threshold **/
    for (i = 0; i < img_pixel_count; i++)
    {
        if(grayscale_img->data[i] <= HOUGH_THRESHOLD)
            continue;

        int angle;
        const size_t y = (i / img_width);
        const size_t x = (i % img_width);
        for (angle = -90; angle < 90; angle++)
        {
            const double theta = fma(DEG2RAD, angle, 0);                           // (M_PI * angle) / 180.0;
            const double rho = fma(x,cos(theta), fma(y,sin(theta), 0));           // (x * cos(theta)) + (y * sin(theta));
            const size_t rho_offset = (size_t) ((round(rho + diag_len) * 180.0));
            houghMatrix.data[rho_offset + angle + 90] += 1;
        }
    }
    normalizeImage(&houghMatrix);
    return houghMatrix;
}

double getPolygonOrientation(Mat *image){
    return -1;
}

void getPolygonSideLengths(Mat *image, double *out_array) {

}

void suppressNeighborsHough(Mat *image, const unsigned int x, const unsigned int y) {

    const size_t height = image->height;
    const size_t width  = image->width;
    double *data = image->data;

    const size_t kernel_width = 180 / KERNEL_MULTIPLIER;
    const size_t kernel_height = height / KERNEL_MULTIPLIER;
    printf("performing analysis for kernel with size : w:%lu, h:%lu \n", kernel_width, kernel_height);

    int xx, yy;
    for(xx = (x - kernel_width/2); xx < (x + kernel_width/2); xx++)
    {
        for(yy = (y - kernel_height/2); yy < (y + kernel_height/2); yy++)
        {
            if(x == xx && y == yy)
                continue;

            const unsigned int idx = (yy * image->width) + xx;
            const double pxl_val = image->data[idx];

            if(pxl_val > 0)
            {
                image->data[idx] = 0.0f;
                printf("Suppressing @ : X:%lu, Y:%lu, XX:%lu, YY:%lu\n", x, y, xx, yy);
            }
        }
    }

}
/** HoughMat is the input here where width = 180 & height = 2 * diagonal distance **/

/**
 * Detects the polygon edge-count by performing Deriche detection on the Hough image now :) and supresses neighbors in
 * a given kernel size. In effect, counts one occurence per kernel, and traverses over the image meaningfully
 * @param image
 * @return
 */
unsigned int getPolygonEdgeCount(Mat *image) {

    Options filterOptions;
    filterOptions.ALPHA_GRADIENT = 1.0;
    filterOptions.ALPHA_BLUR = 10.0;
    filterOptions.HYSTERESIS_THRESHOLD_LOW  = 0.50f * MAGNITUDE_LIMIT;
    filterOptions.HYSTERESIS_THRESHOLD_HIGH = 0.50f * MAGNITUDE_LIMIT;

    *image = edgeDetect(image, filterOptions);

    unsigned int x, y;
    for(x = 0; x < image->width; x++)
    {
        for (y = 0; y < image->height; y++)
        {
            const unsigned int idx = (y * image->width) + x;
            const double pxl_val = image->data[idx];

            if(pxl_val > 0)
            {
                suppressNeighborsHough(image, x, y);
            }
        }
    }

    unsigned int i, numEdgesFound = 0;
    const unsigned int pxl_cnt = image->width * image->height;
    for(i=0; i<pxl_cnt;i++)
    {
        if(image->data[i] > 0)
        {
            numEdgesFound++;
        }
    }

    return numEdgesFound;
}

#endif //HOUGHTRANSFORM_HOUGH_H

//    //return hysteresisThreshold(image, 0.60*MAGNITUDE_LIMIT, 0.65*MAGNITUDE_LIMIT);
//
//    const size_t height = image->height;
//    const size_t width  = image->width;
//    double *data  = image->data;
//
//    const size_t pixel_count = width * height;
//
//    unsigned int numLocalMaxima = 0;
//    size_t x,y;
//
//    for(x = 0; x < width; x++)
//    {
//        for(y = 0; y < height; y++)
//        {
//            const size_t idx = (y * width) + x;
//            const size_t eastPoint = (x == width - 1) ? ((y * width) + 0) : ((y * width) + x + 1);
//            const size_t westPoint = (x == 0) ? ((y * width) + (width - 1)) : ((y * width) + x - 1);
//            const size_t northPoint = (y == height - 1) ? idx : (((y+1) * width) + x);
//            const size_t southPoint = (y == 0) ? x : (((y-1)*width) + x);
//
//            const size_t northEastPoint = northPoint + 1 ;//(x == width - 1) ? ((y * width) + 0) : ((y * width) + x + 1);
//            const size_t northWestPoint = northPoint - 1 ;
//            const size_t southEastPoint = southPoint + 1 ;
//            const size_t southWestPoint = southPoint - 1 ;
//
//            if(data[idx] < MAX_BRIGHTNESS * 0.75)
//                data[idx] = 0.0;
//
//            else if(data[idx] >= data[eastPoint] && data[idx] >= data[westPoint] && data[idx] >= data[northPoint] &&
//                    data[idx] >= data[southPoint] && data[idx] >= data[southWestPoint] &&
//                    data[idx] >= data[southEastPoint] && data[idx] >= data[northWestPoint] && data[idx] >= data[northEastPoint])
//            {
//                //printf("found local maxima at idx:%lu, value is: %f\n", idx, data[idx]);
//                numLocalMaxima++;
//
//                const double _x = y * cos(M_PI * x / 180.0f);
//                const double _y = y * sin(M_PI * x / 180.0f);
//                printf("Maxima @ : theta:%lu , rho:%lu, x:%f, y:%f\n", x,y, _x, _y);
//
//
//                data[eastPoint] = 0.0;
//                data[northPoint] = 0.0;
//
//                data[westPoint] = 0.0;
//                data[southPoint] = 0.0;
//
//                data[southEastPoint] = 0.0;
//                data[southWestPoint] = 0.0;
//
//                data[northEastPoint] = 0.0;
//                data[northWestPoint] = 0.0;
//
//                //data[idx] = 255.0;
//            }
//        }
//    }
////    const size_t kernel_size = 20;//(size_t) sqrt(pixel_count) / 10;
////    const size_t hlf_krnl = kernel_size/2;
////    size_t x, y;
////    double current_max;
////    double running_max_avg = 0;
////    size_t num_max_found = 0;
////
//////    while(num_max_found++ != 4) {
//////        const unsigned int idx = find_next_max(image);
//////        current_max = image->data[idx];
//////        mask_image(image, idx, 20);
//////    }
////    printf("setting kernel size to :%lu\n", height/16);
////
////    while(1)
////    {
////        const unsigned int idx = find_next_max(image);
////        current_max = image->data[idx];
////        mask_image(image, idx, height/8);
////
////        //image->data[idx] = 0.0;
////        if(current_max >= (0.87 * running_max_avg))
////        {
////            running_max_avg = ((running_max_avg * num_max_found++) + current_max) / num_max_found;
////            printf("running_max_avg:%f, num_max_found:%lu, curr_max:%f\n", running_max_avg, num_max_found, current_max);
////        }
////        else
////            break;
////    }
//
////    return numLocalMaxima;
//
//
//typedef struct {
//    size_t pos;
//    double val;
//} Peak;
//
//double getKernelMean(double *data, size_t offset, const size_t width, const size_t pixel_count, const size_t kernel_size) {
//
//    //printf("running get kernel mean!\n");
//    int x, y;
//
//    int hlf_krnl = (int) kernel_size/2;
//    double mean = 0.0;
//
//    Peak localPeak;
//    localPeak.val = 0;
//
//    size_t count = 0;
//
//    for(x = -1 * hlf_krnl; x <= hlf_krnl; x++)
//    {
//        for(y = -1 * hlf_krnl; y <= hlf_krnl; y++)
//        {
//            if(offset + x + (width*y) >= pixel_count)
//            {
//                continue;
//            }
//
//            count++;
//            const double val = *(data + offset + x + (width*y));
//
//            mean += val;
//
//            if(val > localPeak.val)
//            {
//                localPeak.val = val;
//                localPeak.pos = offset + x + (width*y);
//            }
//
//        }
//    }
//    mean /= count;
//
//    if(localPeak.val > 10 * mean && localPeak.val > 25 && localPeak.val !=255.0) {
//        printf("offset:%lu, mean:%f, count:%lu, max:%f\n", offset, mean, count, localPeak.val);
//        *(localPeak.pos + data) = 255.0;
//    }
//
//    //printf("x:%d, y:%d, val:%f\n", localPeak.x, localPeak.y, localPeak.val);
//    return mean;
//}
//
//void mask_image(Mat *image, const size_t idx_start, const int maskSize) {
//
//    const size_t height = image->height;
//    const size_t width = image->width;
//    const size_t pixel_count = width * height;
//
//    int x = idx_start % width;
//    int y = idx_start / width;
//
//    int x_start = (x - 20/2) > 0 ? (x - 20/2) : x;
//    int y_start = (y - maskSize/2) > 0 ? (y - maskSize/2) : y;
//
//    //printf("inside mask image: x_start:%lu, x:%lu, idx_start:%lu, y_start:%lu, y:%lu\n", x_start, x, idx_start, y_start, y);
//
//    for(x_start = (x - maskSize/2) > 0 ? (x - maskSize/2) : x; x_start <= (x + maskSize/2); x_start++)
//    {
//        for(y_start = (y - maskSize/2) > 0 ? (y - maskSize/2) : y; y_start <= (y + maskSize/2); y_start++)
//        {
//            const size_t idx = (y_start * width) + x_start;
//            //printf("masking @ x:%lu, y:%lu, i:%lu\n", x_start, y_start, idx);
//            image->data[idx] = 0;
//        }
//    }
//    return;
//}
//
//
////    size_t x = idx_max % width;
////    size_t y = idx_max % width;
////    for(x = hlf_krnl; x < (width); x+=maskSize)
////    //    for(x = hlf_krnl; x < (width); x+=kernel_size)
//////    {
//////        for(y = hlf_krnl; y < (height); y+=kernel_size)
//////        {
//////            const size_t idx = (y * width) + x;
//////            const double brightness = image->data[idx];
//////            const double max = getKernelMean(image->data, idx, width, pixel_count, kernel_size);
//////
//////            //printf("kernal val:%f\n", max);
////////            if (brightness >= (4 * mean) && mean != 0.0)
////////            {
////////
////////                ///image->data[idx] = 255.0;
////////            }
////////            else
////////                image->data[idx] = 0.0;
//////
//////        }
//////    }
//unsigned int find_next_max(Mat *image) {
//
//    const size_t height = image->height;
//    const size_t width = image->width;
//    const size_t pixel_count = width * height;
//
//    unsigned int i, idx_max = 0;
//    double max = 0;
//    for(i = 0; i < pixel_count; i++)
//    {
//        if(image->data[i] > max)
//        {
//            max = image->data[i];
//            idx_max = i;
//        }
//        //max = (image->data[i] > max) ? (image->data[i]) : max;
//        //idx_max = (image->data[i] > max) ? i : idx_max;
//        //printf("max_found_at:%lu, max_value:%f\n", idx_max, max);
//    }
//    printf("max_found_at:%lu, max_value:%f\n", idx_max, max);
//
//    //image->data[idx_max] = 0.0;
//
//    return idx_max;
//}
