
#define VERBOSE 0

#include <stdio.h>

#include "src/hough.h"

void processImage(char *inputFile, const Options filterOptions, char *grayFile, char *houghFile) {

    /** Step 1 : Read image into Mat in grayscale **/
    Mat grayedImage = imreadGray(inputFile);

    /** Step 2: Deriche Edge Detector
     *
     *     2a: blur image
     *     2b: x-gradient
     *     2c: y-gradient
     *     2d: non-maximum suppression
     *     2e: hysteresis thresholding
     */
    grayedImage = edgeDetect(&grayedImage, filterOptions);

    /** Step 3: Hough Transform **/
    Mat houghImage = HoughTransform(&grayedImage);

    /** Step 4: Get Polygon Edge Count **/
    unsigned int edgeCount = getPolygonEdgeCount(&houghImage);

    /** Step 5: Get Polygon Orientation **/
    const double orientation = getPolygonOrientation(&houghImage);

    /** Step 6: Get Polygon side-lengths **/
    //double *sideLengths;
    //getPolygonSideLengths(&houghImage, sideLengths);

    /* Print results */
    printf("Polygon Edge Count  : %d\n", edgeCount);
    printf("Polygon Orientation : %f\n", orientation);

    /* Optional steps: write to csv */
    writeMat_toCSV(houghFile, houghImage);
    writeMat_toCSV(grayFile, grayedImage);

    destroyMatrix(&grayedImage);
    destroyMatrix(&houghImage);
}

int main() {

    //mem_init();

    /** Hollow Images **/
    Options hollowImageOptions;
    hollowImageOptions.ALPHA_BLUR = 0.0;
    hollowImageOptions.ALPHA_GRADIENT = 0.0;
    hollowImageOptions.HYSTERESIS_THRESHOLD_LOW = 0.0;
    hollowImageOptions.HYSTERESIS_THRESHOLD_HIGH = 0.0;
    //processImage("../bitmaps/image1.bmp", hollowImageOptions, "../outputs/grayscale/image1.csv", "../outputs/hough/image1.csv");
    //processImage("../bitmaps/image2.bmp", hollowImageOptions, "../outputs/grayscale/image2.csv", "../outputs/hough/image2.csv");
    //processImage("../bitmaps/image3.bmp", hollowImageOptions, "../outputs/grayscale/image3.csv", "../outputs/hough/image3.csv");

    /** Hollow Noisy Images **/
    Options hollowNoisyImageOptions;
    hollowNoisyImageOptions.ALPHA_BLUR = 0.0;
    hollowNoisyImageOptions.ALPHA_GRADIENT = 50.0;
    hollowNoisyImageOptions.HYSTERESIS_THRESHOLD_LOW = 100.0;
    hollowNoisyImageOptions.HYSTERESIS_THRESHOLD_HIGH = 150.0;
   //processImage("../bitmaps/noisy2.bmp", hollowNoisyImageOptions, "../outputs/grayscale/noisy2.csv", "../outputs/hough/noisy2.csv");

    /** Hollow - Hand Drawn images **/
    Options hollowHandDrawnImageOptions;
    hollowHandDrawnImageOptions.ALPHA_BLUR = 1.0;
    hollowHandDrawnImageOptions.ALPHA_GRADIENT = 0.0;
    hollowHandDrawnImageOptions.HYSTERESIS_THRESHOLD_LOW = 100.0;
    hollowHandDrawnImageOptions.HYSTERESIS_THRESHOLD_HIGH = 100.0;
    //processImage("../bitmaps/hexagon.bmp",  hollowHandDrawnImageOptions, "../outputs/grayscale/hexagon.csv", "../outputs/hough/hexagon.csv");
    //processImage("../bitmaps/triangle.bmp", hollowHandDrawnImageOptions, "../outputs/grayscale/triangle.csv", "../outputs/hough/triangle.csv");

    /** Solid - well defined polygons **/
    Options solidImageOptions;
    solidImageOptions.ALPHA_BLUR = 200.0;
    solidImageOptions.ALPHA_GRADIENT = 2.0;
    solidImageOptions.HYSTERESIS_THRESHOLD_LOW = 20.0;
    solidImageOptions.HYSTERESIS_THRESHOLD_HIGH = -50.0;
   // processImage("../bitmaps/rubix.bmp", solidImageOptions, "../outputs/grayscale/rubix.csv", "../outputs/hough/rubix.csv");
 ///   processImage("../bitmaps/peacock.bmp", solidImageOptions, "../outputs/grayscale/peacock.csv", "../outputs/hough/peacock.csv");

    /** Solid - Handrawn polygons **/
    Options solidHandDrawnImageOptions;
    solidHandDrawnImageOptions.ALPHA_BLUR = 5.0;
    solidHandDrawnImageOptions.ALPHA_GRADIENT = 1.0;
    solidHandDrawnImageOptions.HYSTERESIS_THRESHOLD_LOW = 10;
    solidHandDrawnImageOptions.HYSTERESIS_THRESHOLD_HIGH = 255;
    //processImage("../bitmaps/diamond.bmp", solidHandDrawnImageOptions, "../outputs/grayscale/diamond.csv", "../outputs/hough/diamond.csv");

    /** Partially filled, handrawn polygon **/
    Options partiallyFilledHandDrawnImageOptions;
    partiallyFilledHandDrawnImageOptions.ALPHA_BLUR = 1.0;
    partiallyFilledHandDrawnImageOptions.ALPHA_GRADIENT = 100.0;
    partiallyFilledHandDrawnImageOptions.HYSTERESIS_THRESHOLD_LOW = 100;
    partiallyFilledHandDrawnImageOptions.HYSTERESIS_THRESHOLD_HIGH = 180;
    processImage("../bitmaps/rick_and_morty.bmp", partiallyFilledHandDrawnImageOptions, "../outputs/grayscale/rick_and_morty.csv", "../outputs/hough/rick_and_morty.csv");

    return 0;
}