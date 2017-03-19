#include <stdio.h>

#include "src/hough.h"
//#include "src/deriche.h"

void processImage(char *inputFile, const double deriche_alpha, const int low_hys, const int high_hys, char *grayFile, char *houghFile) {

    /** Step 1 : Read image into Mat in grayscale **/
    Mat grayedImage = imreadGray(inputFile);
    Mat blurredImage = dericheBlur(&grayedImage, 0.001);
    /* Step 2: Deriche Edge Detector : in-place
     *
     *     2a: blur image
     *     2b: x-gradient
     *     2c: y-gradient
     *     2d: non-maximum suppression
     *     2e: hysteresis thresholding
     */
   // if (deriche_alpha > 0.0) edgeDetect(&grayedImage, deriche_alpha, low_hys, high_hys, 1);

    /* Step 3: Hough Transform */
    //Mat houghImage = HoughTransform(&grayedImage);

    /* Step 4: Get Polygon Edge Count */
    //unsigned int edgeCount = getPolygonEdgeCount(&houghImage);
    //edgeCount = getPolygonEdgeCount(&houghImage);

    /* Step 5: Get Polygon Orientation */
    //const double orientation = getPolygonOrientation(&houghImage);

    /* Step 6: Get Polygon side-lengths */
    //double *sideLengths;
    //getPolygonSideLengths(&houghImage, sideLengths);

    /* Print results */
    //printf("Polygon Edge Count  : %d\n", edgeCount);
    //printf("Polygon Orientation : %f\n", orientation);

    /* Optional steps: write to csv */
   // writeMat_toCSV(houghFile, houghImage);
    writeMat_toCSV(grayFile, blurredImage);

    destroyMatrix(&grayedImage);
   // destroyMatrix(&houghImage);
    destroyMatrix(&blurredImage);
}

int main() {

    mem_init();
    /** Hollow - well defined images **/
    //processImage("../bitmaps/image1.bmp", 0.1, 0, 0, "../outputs/grayscale/image1.csv", "../outputs/hough/image1.csv");
    //processImage("../bitmaps/image1.bmp", 1.0, 10, 20, "../outputs/grayscale/image1.csv", "../outputs/hough/image1.csv");
    //processImage("../bitmaps/image2.bmp", 0.0, 10, 20, "../outputs/grayscale/image2.csv", "../outputs/hough/image2.csv");
    //processImage("../bitmaps/image3.bmp", 000.0, 100, 200, "../outputs/grayscale/image3.csv", "../outputs/hough/image3.csv");

    /** Hollow - Hand drawn images **/
    //processImage("../bitmaps/hexagon.bmp", 0.0, 10, 20, "../outputs/grayscale/hexagon.csv", "../outputs/hough/hexagon.csv");
    //processImage("../bitmaps/triangle.bmp", 0.8, 100, 200, "../outputs/grayscale/triangle.csv", "../outputs/hough/triangle.csv");

    /** Solid - well defined polygons **/
    //processImage("../bitmaps/solid_cube.bmp", 0.0, 100, 150, "../outputs/grayscale/solid_cube.csv", "../outputs/hough/solid_cube.csv");

    /** Solid - Handrawn polygons **/
    //processImage("../bitmaps/diamond.bmp", 1.1, 5, 200, "../outputs/grayscale/diamond.csv", "../outputs/hough/diamond.csv");

    /** Partially filled, handrawn polygon **/
    processImage("../bitmaps/rick_and_morty.bmp", 10, 100, 150, "../outputs/grayscale/rick_and_morty.csv", "../outputs/hough/rick_and_morty.csv");

    return 0;
}