#include <stdio.h>

#include "src/hough.h"
#include "src/deriche.h"


void run(char * path, char* gray_out, char* hough_out, const double alpha) {

    unsigned char *input = imread(path);
    Mat grayedImage = color2gray(input);
    free(input);

    if(alpha >= 0.0) edgeDetect(&grayedImage, alpha);

    Mat houghImage = houghtransform(&grayedImage);

    writeMat_toCSV(hough_out, houghImage);
    writeMat_toCSV(gray_out, grayedImage);

    unsigned int edge_count = detect_polygon_edge_count(&houghImage);
    double orientation_angle = detect_polygon_orientation(&houghImage);


    destroyMatrix(&grayedImage);
    destroyMatrix(&houghImage);

    printf("Polygon Edge Count  : %d\n", edge_count);
    printf("Polygon Orientation : %f\n", orientation_angle);
}

int main() {

    double alpha;

    alpha = -1.0;
    //run("../bitmaps/image1.bmp", "grayscale_image1.csv", "hough_image1.csv", alpha);
    //run("../bitmaps/image2.bmp", "grayscale_image2.csv", "hough_image2.csv", alpha);
    //run("../bitmaps/image3.bmp", "grayscale_image3.csv", "hough_image3.csv", alpha);

    alpha = 1.0;
    //run("../bitmaps/diamond.bmp", "grayscale_diamond.csv", "hough_diamond.csv", alpha);

    run("../bitmaps/triangle.bmp", "grayscale_triangle.csv", "hough_triangle.csv", alpha);

    alpha = -1.0;
    //run("../bitmaps/hexagon.bmp", "grayscale_hexagon.csv", "hough_hexagon.csv", alpha);

    //alpha = 0.4;
    //run("../bitmaps/rick_and_morty.bmp", "grayscale_rick_and_morty.csv", "hough_rick_and_morty.csv", alpha);
    return 0;
}