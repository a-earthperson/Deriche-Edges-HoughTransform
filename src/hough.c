//
// Created by Arjun on 4/30/18.
//

#include "hough.h"

Mat *HoughTransform(const Mat *grayscale_img) {

    size_t i;

    /** get image size parameters */
    const size_t img_width = grayscale_img->width;
    const size_t img_height = grayscale_img->height;
    const size_t img_pixel_count = img_height * img_width;

    /** calculate diagonal length for hough-transformed image **/
    const size_t diag_len = (size_t) hypotf(img_height, img_width);

    /** initialize output matrix with parameters **/
    const size_t width = 180;
    const size_t height = 2 * diag_len;
    //printf("hough image length:%lu, %lu\n", height, width);

    /** create matrix **/
    Mat *houghMatrix = Mat_generate(width, height, 1);

    /** Read read input image pixel values, and perform hough transform on each value that exceeds threshold **/
    for (i = 0; i < img_pixel_count; i++)
    {
        int angle;
        const size_t y = (i / img_width);
        const size_t x = (i % img_width);
        const float magnitude = grayscale_img->data[i];
        for (angle = -90; angle < 90; angle++)
        {
            const float theta = fmaf((float) DEG2RAD, angle, 0);                           // (M_PI * angle) / 180.0;
            const float rho = fmaf(x,cosf(theta), fmaf(y,sinf(theta), 0));           // (x * cos(theta)) + (y * sin(theta));
            const size_t rho_offset = (size_t) ((roundf(rho + diag_len) * 180.0));
            houghMatrix->data[rho_offset + angle + 90] += magnitude;
        }
    }
    return houghMatrix;
}

double getPolygonOrientation(Mat *image){
    return -1;
}

void getPolygonSideLengths(Mat *image, double *out_array) {

}

void suppressNeighborsHough(Mat *image, const size_t x, const size_t y) {

    const size_t height = image->height;
    const size_t kernel_width = 180 / KERNEL_MULTIPLIER;
    const size_t kernel_height = height / KERNEL_MULTIPLIER;
    printf("performing analysis for kernel with size : w:%lu, h:%lu \n", kernel_width, kernel_height);

    size_t xx, yy;
    for(xx = (x - kernel_width/2); xx < (x + kernel_width/2); xx++)
    {
        for(yy = (y - kernel_height/2); yy < (y + kernel_height/2); yy++)
        {
            if(x == xx && y == yy)
                continue;

            const size_t idx = (yy * image->width) + xx;
            const float pxl_val = image->data[idx];

            if(pxl_val > 0)
            {
                image->data[idx] = 0.0f;
                printf("Suppressing @ : X:%lu, Y:%lu, XX:%lu, YY:%lu\n", x, y, xx, yy);
            }
        }
    }

}
