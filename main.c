
#define VERBOSE 1

#include <stdio.h>

#include "src/mat.h"
#include "src/hough.h"
#include "src/deriche.h"

void processImage(char *inputFile, const Options filterOptions) {

    /** Step 1 : Read image into Mat in grayscale **/
   // Mat *srcImage = Mat_generate(, 0);

    Mat *grayedImage = imreadGray(inputFile);
    Mat2CSV(inputFile, grayedImage, ".gray.csv");

    /** Step 2: Deriche Blur **/
    if(filterOptions.ALPHA_BLUR > 0)
    {
        DericheCoeffs *dericheCoeffs = DericheCoeffs_generate(filterOptions.ALPHA_BLUR);
        applyDericheFilter(grayedImage, dericheCoeffs->blur);
        DericheCoeffs_destroy(dericheCoeffs);
        Mat2CSV(inputFile, grayedImage, ".blur.csv");
    }

    /** Step 3 : Gradients! **/
    if(filterOptions.ALPHA_GRADIENT > 0)
    {
        // Prepare two copies of the gray image, one for x-gradient and the other for y-gradient
        Mat *xGradient = Mat_copy(grayedImage);
        Mat *yGradient = grayedImage;

        // Generate the Deriche Coefficients
        DericheCoeffs *gradients = DericheCoeffs_generate(filterOptions.ALPHA_GRADIENT);

        /** Step 3a: X-Gradient **/
        applyDericheFilter(xGradient, gradients->xGradient);
        Mat2CSV(inputFile, xGradient, ".xgrad.csv");

        /** Step 3b : Y-Gradient **/
        applyDericheFilter(yGradient, gradients->yGradient);
        Mat2CSV(inputFile, yGradient, ".ygrad.csv");


        /** Step 3c: Intensity & Gradient calculation with non-maximal supression **/
        // Calculate the gradient intensities, storing the result in yGradient
        calculateGradientIntensities(xGradient, yGradient);
        Mat2CSV(inputFile, grayedImage, ".gradNonMaximalSupression.csv");

        Mat2CSV(inputFile, xGradient, ".gradMagsOnly.csv");

        Mat *houghImage = HoughTransform(xGradient);
        Mat2CSV(inputFile, houghImage, ".hough.csv");

        //free(xGradient->data);
        //xGradient->data = yGradient->dir;

        //Mat2CSV(inputFile, xGradient, ".gradDirs.csv");
        //printf("gradient matrix max: %f\n", grayedImage->max);


        // destroy x-gradient since we dont need it anymore, all the results are in yGradient = grayedImage
        //Mat_destroy(xGradient);

        DericheCoeffs_destroy(gradients);
    }

    /** Step 5: Hysteresis Threshold **/
    if(filterOptions.HYSTERESIS_THRESHOLD_LOW > 0 && filterOptions.HYSTERESIS_THRESHOLD_HIGH > filterOptions.HYSTERESIS_THRESHOLD_LOW)
    {
        hysteresisThreshold(grayedImage, filterOptions.HYSTERESIS_THRESHOLD_LOW, filterOptions.HYSTERESIS_THRESHOLD_HIGH);
        Mat2CSV(inputFile, grayedImage, ".hysteresis.csv");
    }


    //Mat *houghImage = HoughTransform(grayedImage);
    //Mat2CSV(inputFile, houghImage, ".hough.csv");

    /** Step 4: Non-Maximal supression **/
   // performNonMaximalMagnitudeSupression(grayedImage, grayedImage->max);



//    /** Step 3: Gradients **/
//    if(filterOptions.ALPHA_GRADIENT > 0)
//    {
//        DericheCoeffs *gradientCoeffs = DericheCoeffs_generate(filterOptions.ALPHA_GRADIENT);
//        Mat *xyGradientImage = Mat_generate(grayedImage->width, grayedImage->height, 0);
//        applyDericheFilter(grayedImage, xyGradientImage, gradientCoeffs->xGradient);
//        applyDericheFilter(xyGradientImage, grayedImage, gradientCoeffs->yGradient);
//        DericheCoeffs_destroy(gradientCoeffs);
//        destroyGradMatrix(xyGradientImage);
//    }

    /** Step 2: Deriche Edge Detector
     *
     *     2a: blur image
     *     2b: x-gradient
     *     2c: y-gradient
     *     2d: non-maximum suppression
     *     2e: hysteresis thresholding
     */
    //grayedImage = edgeDetect(&grayedImage, filterOptions);

    //writeMat_toCSV(houghFile, grayedImage);

    /** Step 3: Hough Transform **/
    //Mat *houghImage = HoughTransform(grayedImage);
    //Mat2CSV(inputFile, houghImage, ".hough.csv");
    //Mat_destroy(houghImage);

    /** Step 4: Get Polygon Edge Count **/
    //unsigned int edgeCount = getPolygonEdgeCount(&houghImage);

    /** Step 5: Get Polygon Orientation **/
    //const double orientation = getPolygonOrientation(&houghImage);

    /** Step 6: Get Polygon side-lengths **/
    //double *sideLengths;
    //getPolygonSideLengths(&houghImage, sideLengths);

    /* Print results */
    //printf("Polygon Edge Count  : %d\n", edgeCount);
    //printf("Polygon Orientation : %f\n", orientation);

    Mat_destroy(grayedImage);
}

int main() {

    //mem_init();

    /** Hollow Images **/
  // Options hollowImageOptions;
  //  hollowImageOptions.ALPHA_BLUR = 0;
  //  hollowImageOptions.ALPHA_GRADIENT = 10;
   // hollowImageOptions.HYSTERESIS_THRESHOLD_LOW = 0;
   // hollowImageOptions.HYSTERESIS_THRESHOLD_HIGH = 0;
    //processImage("../examples/sunflower.bmp", hollowImageOptions);
    //processImage("../examples/image2.bmp", hollowImageOptions);
    //processImage("../examples/rubix.bmp", hollowImageOptions);
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
    solidHandDrawnImageOptions.ALPHA_BLUR = 1.0;
    solidHandDrawnImageOptions.ALPHA_GRADIENT = 1.0;
    solidHandDrawnImageOptions.HYSTERESIS_THRESHOLD_LOW = 0;
    solidHandDrawnImageOptions.HYSTERESIS_THRESHOLD_HIGH = 0;
      processImage("../examples/diamond.bmp", solidHandDrawnImageOptions);
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