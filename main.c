
#define VERBOSE 1

#include <stdio.h>

#include "include/mat.h"
#include "include/hough.h"
#include "include/deriche.h"
#include "include/otsu.h"


void processImage(char *inputFile, const Options filterOptions) {

    /** Step 1 : Read BMP image as gray-scale 2D Mat **/
    Mat *grayedImage = imreadGray(inputFile);
    if(VERBOSE) Mat2CSV(inputFile, grayedImage, ".gray.csv");

    /** Step 2: Deriche Blur Filter **/
    if(filterOptions.ALPHA_BLUR > 0)
    {
        DericheCoeffs *dericheCoeffs = DericheCoeffs_generate(filterOptions.ALPHA_BLUR);
        applyDericheFilter(grayedImage, dericheCoeffs->blur);
        DericheCoeffs_destroy(dericheCoeffs);
        if(VERBOSE) Mat2CSV(inputFile, grayedImage, ".blur.csv");
    }

    /** Step 3: Otsu Threshold **/
    unsigned int threshold = computeThreshold(grayedImage);
    printf("Otsu threshold:%u\n", threshold);
    if(VERBOSE) Mat2CSV(inputFile, grayedImage, ".otsu.csv");

    /** Step 4 : Gradients! **/
    if(filterOptions.ALPHA_GRADIENT > 0)
    {
        // Prepare two copies of the gray image, one for x-gradient and the other for y-gradient
        Mat *xGradient = Mat_copy(grayedImage);
        Mat *yGradient = grayedImage;

        // Generate the Deriche Coefficients
        DericheCoeffs *gradients = DericheCoeffs_generate(filterOptions.ALPHA_GRADIENT);
        if(VERBOSE) writeDericheCoeffs(gradients);

        /** Step 3a: X-Gradient **/
        applyDericheFilter(xGradient, gradients->xGradient);
        if(VERBOSE) Mat2CSV(inputFile, xGradient, ".xgrad.csv");

        /** Step 3b : Y-Gradient **/
        applyDericheFilter(yGradient, gradients->yGradient);
        if(VERBOSE) Mat2CSV(inputFile, yGradient, ".ygrad.csv");

        /** Step 3c :  apply element-wise hypot function **/
        Mat_elementwise(xGradient, yGradient, &hypotf);
        if(VERBOSE) Mat2CSV(inputFile, grayedImage, ".gradOnly.csv");

        Mat_destroy(xGradient);
        DericheCoeffs_destroy(gradients);
    }

    /** Step 4: Hysteresis Threshold **/
    if(filterOptions.HYSTERESIS_THRESHOLD_LOW > 0 && filterOptions.HYSTERESIS_THRESHOLD_HIGH > filterOptions.HYSTERESIS_THRESHOLD_LOW)
    {
        hysteresisThreshold(grayedImage, filterOptions.HYSTERESIS_THRESHOLD_LOW, filterOptions.HYSTERESIS_THRESHOLD_HIGH);
        if(VERBOSE) Mat2CSV(inputFile, grayedImage, ".hysteresis.csv");
    }

    /** Step 5a: Linear Hough Transform **/
    Mat *houghImage = HoughTransform(grayedImage);
    if(VERBOSE) Mat2CSV(inputFile, houghImage, ".hough.csv");

    /** Step 5b: Linear Hough Transform **/
    Mat_elementwise(houghImage, houghImage, &multipy);
    normalizeImage(houghImage);
    if(VERBOSE) Mat2CSV(inputFile, houghImage, ".hough-powers.csv");

    /** Step 6: Polygon edge count **/
    printf("Polygon Edge Count  : %d\n", -1);

    /** Step 7: Polygon orientation **/
    printf("Polygon Orientation : %f\n", -1.0f);

    Mat_destroy(houghImage);
    Mat_destroy(grayedImage);
}

int main() {
    
    Options imageOptions;
    imageOptions.HYSTERESIS_THRESHOLD_LOW = 0;
    imageOptions.HYSTERESIS_THRESHOLD_HIGH = 0;

    /** Hollow polygons **/
    imageOptions.ALPHA_BLUR = 0;
    imageOptions.ALPHA_GRADIENT = 0;
    processImage("./examples/image1.bmp", imageOptions);
    processImage("./examples/image2.bmp", imageOptions);
    processImage("./examples/image3.bmp", imageOptions);

    /** Hollow Noisy (Handrawn) polygons **/
    imageOptions.ALPHA_BLUR = 2;
    imageOptions.ALPHA_GRADIENT = 0;
    processImage("./examples/triangle.bmp", imageOptions);
    processImage("./examples/hexagon.bmp", imageOptions);

    /** Solid - Handrawn polygons **/
    imageOptions.ALPHA_BLUR = 1;
    imageOptions.ALPHA_GRADIENT = 10;
    processImage("./examples/diamond.bmp", imageOptions);

    return 0;
}
