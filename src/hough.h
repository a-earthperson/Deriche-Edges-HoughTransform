//
// Created by Arjun on 2/20/17.
//

#ifndef HOUGHTRANSFORM_HOUGH_H
#define HOUGHTRANSFORM_HOUGH_H

#define MAX_BRIGHTNESS 255.0
#define MIN_BRIGHTNESS 0.0

#define HOUGH_THRESHOLD 1

#define NORMALIZE_ALPHA 1

#define VERBOSE 0

#include <math.h>
#include "bmp.h"
#include "helper.h"
#include "deriche.h"


void performHough(size_t x, size_t y, double brightness, size_t diag_len,  double *accumulator) {

    int angle;
    for (angle = -90; angle < 90; angle++)
    {
        const double theta = (M_PI * angle) / 180.0;
        const double rho = (x * cos(theta)) + (y * sin(theta));

        const size_t rho_offset = (size_t) ((round(rho + diag_len) * 180.0));
        const size_t angle_offset = angle + 90;

        *(accumulator + rho_offset + angle_offset) += (1);

        if(VERBOSE) printf("rho: %f, theta :%d, rho_offset:%lu, angle_offset:%lu, acc:%f\n", rho, angle, rho_offset, angle_offset, *(accumulator + rho_offset + angle_offset));
    }
}

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

    /* get image size parameters */
    const size_t img_width = grayscale_img->width;
    const size_t img_height = grayscale_img->height;
    const size_t img_pixel_count = img_height * img_width;

    /* calculate diagonal length for hough-transformed image */
    const size_t diagonal_length = (size_t) hypot(img_height, img_width);

    /* initialize output matrix with parameters */
    const size_t width = 180;
    const size_t height = 2 * diagonal_length;

    printf("hough image length:%lu, %lu\n", diagonal_length, width);

    // create matrix, pass 1 to initialize it as empty
    Mat houghMatrix = createMatrix(width, height, 1);

    /* Read read input image pixel values, and perform hough transform on each value that exceeds threshold */
    for (i = 0; i < img_pixel_count; i++)
    {
        const double pixel_value = grayscale_img->data[i];
        ///printf("pixel_value:%f\n", pixel_value);
        if (pixel_value > HOUGH_THRESHOLD)
        {
            const size_t y = (i / img_width);
            const size_t x = (i % img_width);
            performHough(x, y, pixel_value, diagonal_length, houghMatrix.data);
        }
    }
    normalizeImage(&houghMatrix);
    //edgeDetect(&houghMatrix, 1.0, 0.0, 0.0, 1.0);
    return houghMatrix;
}

int cmpfunc (const void * a, const void * b)
{
//    printf("a:%f, b:%f\n", *(double*)a, *(double*)b);
    return (int)( *(double*)a - *(double*)b );
}

typedef struct {
    size_t pos;
    double val;
} Peak;

double getKernelMean(double *data, size_t offset, const size_t width, const size_t pixel_count, const size_t kernel_size) {

    //printf("running get kernel mean!\n");
    int x, y;

    int hlf_krnl = (int) kernel_size/2;
    double mean = 0.0;

    Peak localPeak;
    localPeak.val = 0;

    size_t count = 0;

    for(x = -1 * hlf_krnl; x <= hlf_krnl; x++)
    {
        for(y = -1 * hlf_krnl; y <= hlf_krnl; y++)
        {
            if(offset + x + (width*y) >= pixel_count)
            {
                continue;
            }

            count++;
            const double val = *(data + offset + x + (width*y));

            mean += val;

            if(val > localPeak.val)
            {
                localPeak.val = val;
                localPeak.pos = offset + x + (width*y);
            }

        }
    }
    mean /= count;

    if(localPeak.val > 10 * mean && localPeak.val > 25 && localPeak.val !=255.0) {
        printf("offset:%lu, mean:%f, count:%lu, max:%f\n", offset, mean, count, localPeak.val);
        *(localPeak.pos + data) = 255.0;
    }

    //printf("x:%d, y:%d, val:%f\n", localPeak.x, localPeak.y, localPeak.val);
    return mean;
}

void mask_image(Mat *image, const size_t idx_start, const int maskSize) {

    const size_t height = image->height;
    const size_t width = image->width;
    const size_t pixel_count = width * height;

    int x = idx_start % width;
    int y = idx_start / width;

    int x_start = (x - 20/2) > 0 ? (x - 20/2) : x;
    int y_start = (y - maskSize/2) > 0 ? (y - maskSize/2) : y;

    //printf("inside mask image: x_start:%lu, x:%lu, idx_start:%lu, y_start:%lu, y:%lu\n", x_start, x, idx_start, y_start, y);

    for(x_start = (x - maskSize/2) > 0 ? (x - maskSize/2) : x; x_start <= (x + maskSize/2); x_start++)
    {
        for(y_start = (y - maskSize/2) > 0 ? (y - maskSize/2) : y; y_start <= (y + maskSize/2); y_start++)
        {
            const size_t idx = (y_start * width) + x_start;
            //printf("masking @ x:%lu, y:%lu, i:%lu\n", x_start, y_start, idx);
            image->data[idx] = 0;
        }
    }
    return;
}


//    size_t x = idx_max % width;
//    size_t y = idx_max % width;
//    for(x = hlf_krnl; x < (width); x+=maskSize)
//    //    for(x = hlf_krnl; x < (width); x+=kernel_size)
////    {
////        for(y = hlf_krnl; y < (height); y+=kernel_size)
////        {
////            const size_t idx = (y * width) + x;
////            const double brightness = image->data[idx];
////            const double max = getKernelMean(image->data, idx, width, pixel_count, kernel_size);
////
////            //printf("kernal val:%f\n", max);
//////            if (brightness >= (4 * mean) && mean != 0.0)
//////            {
//////
//////                ///image->data[idx] = 255.0;
//////            }
//////            else
//////                image->data[idx] = 0.0;
////
////        }
////    }
unsigned int find_next_max(Mat *image) {

    const size_t height = image->height;
    const size_t width = image->width;
    const size_t pixel_count = width * height;

    unsigned int i, idx_max = 0;
    double max = 0;
    for(i = 0; i < pixel_count; i++)
    {
        if(image->data[i] > max)
        {
            max = image->data[i];
            idx_max = i;
        }
        //max = (image->data[i] > max) ? (image->data[i]) : max;
        //idx_max = (image->data[i] > max) ? i : idx_max;
        //printf("max_found_at:%lu, max_value:%f\n", idx_max, max);
    }
    printf("max_found_at:%lu, max_value:%f\n", idx_max, max);

    //image->data[idx_max] = 0.0;

    return idx_max;
}

double getPolygonOrientation(Mat *image){
    return -1;
}

void getPolygonSideLengths(Mat *image, double *out_array) {

}

/** HoughMat is the input here where width = 180 & height = 2 * diagonal distance **/
unsigned int getPolygonEdgeCount(Mat *image){

    const size_t height = image->height;
    const size_t width  = image->width;
    double *data  = image->data;

    const size_t pixel_count = width * height;

    unsigned int numLocalMaxima = 0;
    size_t x,y;

    for(x = 0; x < width; x++)
    {
        for(y = 0; y < height; y++)
        {
            const size_t idx = (y * width) + x;
            const size_t eastPoint = (x == width - 1) ? ((y * width) + 0) : ((y * width) + x + 1);
            const size_t westPoint = (x == 0) ? ((y * width) + (width - 1)) : ((y * width) + x - 1);
            const size_t northPoint = (y == height - 1) ? idx : (((y+1) * width) + x);
            const size_t southPoint = (y == 0) ? x : (((y-1)*width) + x);

            const size_t northEastPoint = northPoint + 1 ;//(x == width - 1) ? ((y * width) + 0) : ((y * width) + x + 1);
            const size_t northWestPoint = northPoint - 1 ;
            const size_t southEastPoint = southPoint + 1 ;
            const size_t southWestPoint = southPoint - 1 ;

            if(data[idx] < MAX_BRIGHTNESS * 0.75)
                data[idx] = 0.0;

            else if(data[idx] >= data[eastPoint] && data[idx] >= data[westPoint] && data[idx] >= data[northPoint] &&
                    data[idx] >= data[southPoint] && data[idx] >= data[southWestPoint] &&
                    data[idx] >= data[southEastPoint] && data[idx] >= data[northWestPoint] && data[idx] >= data[northEastPoint])
            {
                printf("found local maxima at idx:%lu, value is: %f\n", idx, data[idx]);
                numLocalMaxima++;

                data[eastPoint] = 0.0;
                data[northPoint] = 0.0;

                data[westPoint] = 0.0;
                data[southPoint] = 0.0;

                data[southEastPoint] = 0.0;
                data[southWestPoint] = 0.0;

                data[northEastPoint] = 0.0;
                data[northWestPoint] = 0.0;

                //data[idx] = 255.0;
            }
        }
    }
//    const size_t kernel_size = 20;//(size_t) sqrt(pixel_count) / 10;
//    const size_t hlf_krnl = kernel_size/2;
//    size_t x, y;
//    double current_max;
//    double running_max_avg = 0;
//    size_t num_max_found = 0;
//
////    while(num_max_found++ != 4) {
////        const unsigned int idx = find_next_max(image);
////        current_max = image->data[idx];
////        mask_image(image, idx, 20);
////    }
//    printf("setting kernel size to :%lu\n", height/16);
//
//    while(1)
//    {
//        const unsigned int idx = find_next_max(image);
//        current_max = image->data[idx];
//        mask_image(image, idx, height/8);
//
//        //image->data[idx] = 0.0;
//        if(current_max >= (0.87 * running_max_avg))
//        {
//            running_max_avg = ((running_max_avg * num_max_found++) + current_max) / num_max_found;
//            printf("running_max_avg:%f, num_max_found:%lu, curr_max:%f\n", running_max_avg, num_max_found, current_max);
//        }
//        else
//            break;
//    }

    return numLocalMaxima;
}
//
//    printf("Since pixel_count is %lu, kernel size is :%lu\n", pixel_count, kernel_size);
//
//    for(x = hlf_krnl; x < (width); x+=kernel_size)
//    {
//        for(y = hlf_krnl; y < (height); y+=kernel_size)
//        {
//            const size_t idx = (y * width) + x;
//            const double brightness = image->data[idx];
//            const double max = getKernelMean(image->data, idx, width, pixel_count, kernel_size);
//
//            //printf("kernal val:%f\n", max);
////            if (brightness >= (4 * mean) && mean != 0.0)
////            {
////
////                ///image->data[idx] = 255.0;
////            }
////            else
////                image->data[idx] = 0.0;
//
//        }
//    }
//    //qsort(image->data, pixel_count, sizeof(double), cmpfunc);
//    //double median = image->data[0];
//
//    const double mean = 0.0;//sum/(pixel_count);
//    int count = 0;
//    for(x = 0; x < pixel_count; x++)
//    {
//        count = (image->data[x] > mean * 51.0) ? count+1 : count;
//    }
//    //printf("max:%f, sum:%f, mean:%f, median:%f, count:%d\n", max, sum, mean, median, count);
//    return count;
#endif //HOUGHTRANSFORM_HOUGH_H
//Mat color2gray(Bitmap *image) {
//
//    size_t i;
//    const size_t width = image->width;
//    const size_t height = image->height;
//
//    double most_bright_pixel = MIN_BRIGHTNESS;
//    const size_t pixel_count = (width * height);
//
//    Mat grayImage = createMatrix(width, height, 0);
//
//    /* calculate brightness for all pixels & find the brightest pixel */
//    for(i = 0; i < pixel_count; i++)
//    {
//        size_t idx = 3*i;
//        const double R = image->data[idx++];  // 3i + 0
//        const double G = image->data[idx++];  // 3i + 1
//        const double B = image->data[idx];    // 3i + 2
//
//        /* store image magnitude in gray image mat */
//        grayImage.data[i] = sqrt((R*R) + (G*G) + (B*B));
//
//        /* find the brightest pixel */
//        most_bright_pixel = (grayImage.data[i] > most_bright_pixel) ? grayImage.data[i] : most_bright_pixel;
//    }
//
//    /** calculate normalization factor alpha **/
//    if(NORMALIZE_ALPHA)
//    {
//        const double ALPHA = (MAX_BRIGHTNESS / most_bright_pixel);
//        for (i = 0; i < pixel_count; i++)
//        {
//            grayImage.data[i] = MAX_BRIGHTNESS - (grayImage.data[i] * ALPHA);
//            if(VERBOSE) printf("gray:%f\n", grayImage.data[i]);
//        }
//    }
//
//    return grayImage;
//}