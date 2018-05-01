
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

    unsigned int threshold = computeThreshold(grayedImage);
    printf("Otsu threshold:%u\n", threshold);
    if(VERBOSE) Mat2CSV(inputFile, grayedImage, ".otsu.csv");

    /** Step 3 : Gradients! **/
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


        /** Step 3c: Intensity & Gradient calculation with non-maximal supression **/
        // Calculate the gradient intensities, storing the result in yGradient
        //alculateGradientIntensities(xGradient, yGradient);
        performMagnitudeSupression(xGradient, yGradient);
        if(VERBOSE) Mat2CSV(inputFile, xGradient, ".gradOnly.csv");
        if(VERBOSE) Mat2CSV(inputFile, yGradient, ".gradNonMaximal.csv");


        grayedImage = xGradient;

        //xGradient = grayedImage;
        //Mat_destroy(xGradient);

        //free(xGradient->data);
        //xGradient->data = yGradient->dir;

        //Mat2CSV(inputFile, xGradient, ".gradDirs.csv");
        //printf("gradient matrix max: %f\n", grayedImage->max);


        // destroy x-gradient since we dont need it anymore, all the results are in yGradient = grayedImage
        //Mat_destroy(xGradient);

        DericheCoeffs_destroy(gradients);
    }

    /** Step 4: Hysteresis Threshold **/
    if(filterOptions.HYSTERESIS_THRESHOLD_LOW > 0 && filterOptions.HYSTERESIS_THRESHOLD_HIGH > filterOptions.HYSTERESIS_THRESHOLD_LOW)
    {
        hysteresisThreshold(grayedImage, filterOptions.HYSTERESIS_THRESHOLD_LOW, filterOptions.HYSTERESIS_THRESHOLD_HIGH);
        if(VERBOSE) Mat2CSV(inputFile, grayedImage, ".hysteresis.csv");
    }

    /** Step 5: Linear Hough Transform **/
    Mat *houghImage = HoughTransform(grayedImage);
    if(VERBOSE) Mat2CSV(inputFile, houghImage, ".hough.csv");


    Mat *xHough = Mat_copy(houghImage);
    applyDericheFilter(xHough, DericheCoeffs_generate(100)->xGradient);

    Mat *yHough = Mat_copy(houghImage);
    applyDericheFilter(yHough, DericheCoeffs_generate(100)->yGradient);

    performMagnitudeSupression(xHough, yHough);
    computeThreshold(xHough);
    Mat_multiply(houghImage, xHough);
    if(VERBOSE) Mat2CSV(inputFile, xHough, ".hough-gradOnly.csv");

    /** Step 6: Polygon edge count **/
    printf("Polygon Edge Count  : %d\n", -1);

    /** Step 7: Polygon orientation **/
    printf("Polygon Orientation : %f\n", -1.0f);


    Mat_destroy(houghImage);
    Mat_destroy(grayedImage);
}

int main() {

    //mem_init();

    /** Hollow Images **/
    Options hollowImageOptions;
    hollowImageOptions.ALPHA_BLUR = 0;
    hollowImageOptions.ALPHA_GRADIENT = 0;
    hollowImageOptions.HYSTERESIS_THRESHOLD_LOW = 0;
    hollowImageOptions.HYSTERESIS_THRESHOLD_HIGH = 0;
    //processImage("./examples/image1.bmp", hollowImageOptions);
    //processImage("../examples/image2.bmp", hollowImageOptions);
    processImage("./examples/image3.bmp", hollowImageOptions);
    //processImage("../bitmaps/image2.bmp", hollowImageOptions, "../outputs/grayscale/image2.csv", "../outputs/hough/image2.csv");
    //processImage("../bitmaps/image3.bmp", hollowImageOptions, "../outputs/grayscale/image3.csv", "../outputs/hough/image3.csv");

    /** Hollow Noisy Images **/
    Options hollowNoisyImageOptions;
    hollowNoisyImageOptions.ALPHA_BLUR = 0.0;
    hollowNoisyImageOptions.ALPHA_GRADIENT = 50.0;
    hollowNoisyImageOptions.HYSTERESIS_THRESHOLD_LOW = 100;
    hollowNoisyImageOptions.HYSTERESIS_THRESHOLD_HIGH = 150;
    //processImage("../bitmaps/noisy2.bmp", hollowNoisyImageOptions, "../outputs/grayscale/noisy2.csv", "../outputs/hough/noisy2.csv");

    /** Hollow - Hand Drawn images **/
    Options hollowHandDrawnImageOptions;
    hollowHandDrawnImageOptions.ALPHA_BLUR = 1.0;
    hollowHandDrawnImageOptions.ALPHA_GRADIENT = 0.0;
    hollowHandDrawnImageOptions.HYSTERESIS_THRESHOLD_LOW = 100;
    hollowHandDrawnImageOptions.HYSTERESIS_THRESHOLD_HIGH = 100;
    //processImage("../bitmaps/hexagon.bmp",  hollowHandDrawnImageOptions, "../outputs/grayscale/hexagon.csv", "../outputs/hough/hexagon.csv");
    //processImage("../bitmaps/triangle.bmp", hollowHandDrawnImageOptions, "../outputs/grayscale/triangle.csv", "../outputs/hough/triangle.csv");

    /** Solid - well defined polygons **/
    Options solidImageOptions;
    solidImageOptions.ALPHA_BLUR = 200.0;
    solidImageOptions.ALPHA_GRADIENT = 2.0;
    solidImageOptions.HYSTERESIS_THRESHOLD_LOW = 20;
    solidImageOptions.HYSTERESIS_THRESHOLD_HIGH = 50;
    //processImage("../bitmaps/rubix.bmp", solidImageOptions, "../outputs/grayscale/rubix.csv", "../outputs/hough/rubix.csv");
    ///processImage("../bitmaps/peacock.bmp", solidImageOptions, "../outputs/grayscale/peacock.csv", "../outputs/hough/peacock.csv");

    /** Solid - Handrawn polygons **/
    Options solidHandDrawnImageOptions;
    solidHandDrawnImageOptions.ALPHA_BLUR = 0.5;
    solidHandDrawnImageOptions.ALPHA_GRADIENT = 10.0;
    solidHandDrawnImageOptions.HYSTERESIS_THRESHOLD_LOW = 0;
    solidHandDrawnImageOptions.HYSTERESIS_THRESHOLD_HIGH = 0;
    //processImage("../examples/diamond.bmp", solidHandDrawnImageOptions);
//    processImage("../bitmaps/diamond.bmp", solidHandDrawnImageOptions, "../outputs/grayscale/diamond.csv", "../outputs/hough/diamond.csv");

    /** Partially filled, handrawn polygon **/
    Options partiallyFilledHandDrawnImageOptions;
    partiallyFilledHandDrawnImageOptions.ALPHA_BLUR = 1.0;
    partiallyFilledHandDrawnImageOptions.ALPHA_GRADIENT = 100.0;
    partiallyFilledHandDrawnImageOptions.HYSTERESIS_THRESHOLD_LOW = 100;
    partiallyFilledHandDrawnImageOptions.HYSTERESIS_THRESHOLD_HIGH = 180;
    //processImage("../bitmaps/rick_and_morty.bmp", partiallyFilledHandDrawnImageOptions, "../outputs/grayscale/rick_and_morty.csv", "../outputs/hough/rick_and_morty.csv");

    return 0;
}
