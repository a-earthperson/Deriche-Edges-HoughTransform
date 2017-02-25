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
    run("../bitmaps/image1.bmp", "../outputs/grayscale_image1.csv", "../outputs/hough_image1.csv", alpha);
    run("../bitmaps/image2.bmp", "../outputs/grayscale_image2.csv", "../outputs/hough_image2.csv", alpha);
    run("../bitmaps/image3.bmp", "../outputs/grayscale_image3.csv", "../outputs/hough_image3.csv", alpha);

    alpha = 1.0;
    run("../bitmaps/diamond.bmp", "../outputs/grayscale_diamond.csv", "../outputs/hough_diamond.csv", alpha);

    run("../bitmaps/triangle.bmp", "../outputs/grayscale_triangle.csv", "../outputs/hough_triangle.csv", alpha);

    alpha = -1.0;
    run("../bitmaps/hexagon.bmp", "../outputs/grayscale_hexagon.csv", "../outputs/hough_hexagon.csv", alpha);

    alpha = 0.4;
    run("../bitmaps/rick_and_morty.bmp", "../outputs/grayscale_rick_and_morty.csv", "../outputs/hough_rick_and_morty.csv", alpha);
    return 0;
}