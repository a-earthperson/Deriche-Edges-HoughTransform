
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "mat.h"
#include "hough.h"
#include "deriche.h"
#include "otsu.h"

static int VERBOSE = 0;
static int WRITE_CSV = 0;
static int OTSU_THRESHOLD = 1;

void processImage(char *inputFile, const Options* filterOptions) {

    /** Step 1 : Read BMP image as gray-scale 2D Mat **/
    Mat *grayedImage = imreadGray(inputFile);
    if(WRITE_CSV) Mat2CSV(inputFile, grayedImage, ".gray.csv");

    /** Step 2: Deriche Blur Filter **/
    if(filterOptions->ALPHA_BLUR > 0)
    {
        DericheCoeffs *dericheCoeffs = DericheCoeffs_generate(filterOptions->ALPHA_BLUR);
        applyDericheFilter(grayedImage, dericheCoeffs->blur);
        DericheCoeffs_destroy(dericheCoeffs);
        if(WRITE_CSV) Mat2CSV(inputFile, grayedImage, ".blur.csv");
    }

    /** Step 3: Otsu Threshold **/
    if(OTSU_THRESHOLD)
    {
        unsigned int threshold = computeThreshold(grayedImage);
        if(VERBOSE) printf("Otsu threshold:%u\n", threshold);
        if(WRITE_CSV) Mat2CSV(inputFile, grayedImage, ".otsu.csv");
    }

    /** Step 4 : Gradients! **/
    if(filterOptions->ALPHA_GRADIENT > 0)
    {
        // Prepare two copies of the gray image, one for x-gradient and the other for y-gradient
        Mat *xGradient = Mat_copy(grayedImage);
        Mat *yGradient = grayedImage;

        // Generate the Deriche Coefficients
        DericheCoeffs *gradients = DericheCoeffs_generate(filterOptions->ALPHA_GRADIENT);
        if(VERBOSE) writeDericheCoeffs(gradients);

        /** Step 3a: X-Gradient **/
        applyDericheFilter(xGradient, gradients->xGradient);
        if(WRITE_CSV) Mat2CSV(inputFile, xGradient, ".xgrad.csv");

        /** Step 3b : Y-Gradient **/
        applyDericheFilter(yGradient, gradients->yGradient);
        if(WRITE_CSV) Mat2CSV(inputFile, yGradient, ".ygrad.csv");

        /** Step 3c :  apply element-wise hypot function **/
        Mat_elementwise2(xGradient, yGradient, &hypotf);
        Mat_destroy(xGradient);

        //suppressThreshold(yGradient, 1);
        printf("Otsu 2nd threshold:%d\n", computeThreshold(grayedImage));
        if(WRITE_CSV) Mat2CSV(inputFile, grayedImage, ".gradOnly.csv");


        DericheCoeffs_destroy(gradients);
    }

    /** Step 4: Hysteresis Threshold **/
    if(filterOptions->HYSTERESIS_THRESHOLD_LOW > 0 && filterOptions->HYSTERESIS_THRESHOLD_HIGH > filterOptions->HYSTERESIS_THRESHOLD_LOW)
    {
        hysteresisThreshold(grayedImage, filterOptions->HYSTERESIS_THRESHOLD_LOW, filterOptions->HYSTERESIS_THRESHOLD_HIGH);
        if(WRITE_CSV) Mat2CSV(inputFile, grayedImage, ".hysteresis.csv");
    }

    /** Step 5a: Linear Hough Transform **/
    Mat *houghImage = HoughTransform(grayedImage);
    //normalizeImage(houghImage);
    if(WRITE_CSV) Mat2CSV(inputFile, houghImage, ".hough.csv");

    size_t n = houghImage->width * houghImage->height;
    size_t i;
    for(i = 0; i < n; i++)
    {
        houghImage->data[i] = expf(0.01f * houghImage->data[i]);
    }
    /** Step 5b: Linear Hough Transform **/
   // Mat_elementwise2(houghImage, houghImage, multipy);
    //normalizeImage(houghImage);
    if(WRITE_CSV) Mat2CSV(inputFile, houghImage, ".hough-powers.csv");

    //Mat_elementwise1(houghImage, &sq);
   // //computeThreshold(houghImage);
    //normalizeImage(houghImage);
    //normalizeImage(houghImage);
    //if(WRITE_CSV) Mat2CSV(inputFile, houghImage, ".hough-log2.csv");

    /** Step 6: Polygon edge count **/
    printf("Polygon Edge Count  : %d\n", -1);

    /** Step 7: Polygon orientation **/
    printf("Polygon Orientation : %f\n", -1.0f);

    Mat_destroy(houghImage);
    Mat_destroy(grayedImage);
}

int main(int argc, char **argv) {


    Options imageOptions;
    imageOptions.ALPHA_BLUR = 0;
    imageOptions.ALPHA_GRADIENT = 100;
    imageOptions.HYSTERESIS_THRESHOLD_LOW = 0;
    imageOptions.HYSTERESIS_THRESHOLD_HIGH = 0;

    int c;

    while (1)
    {
        static struct option long_options[] =
                {
                        /* These options set a flag. */
                        {"verbose",                no_argument,        &VERBOSE, 1},
                        {"write-csv",              no_argument,      &WRITE_CSV, 1},
                        {"disable-ostu-threshold", no_argument, &OTSU_THRESHOLD, 0},

                        /* These options don’t set a flag. We distinguish them by their indices. */
                        {"set-blur-alpha",      required_argument, 0, 'b'},
                        {"set-gradient-alpha",  required_argument, 0, 'g'},
                        {"set-hysteresis-low",  required_argument, 0, 'l'},
                        {"set-hysteresis-high", required_argument, 0, 'h'},
                        {0, 0, 0, 0}
                };

        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long(argc, argv, "abc:d:f:", long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c)
        {
            case 0:
                /* If this option set a flag, do nothing else now. */
                if (long_options[option_index].flag != 0)
                    break;

                printf ("option %s", long_options[option_index].name);
                if (optarg)
                    printf (" with arg %s", optarg);
                printf ("\n");
                break;

            case 'b':
                imageOptions.ALPHA_BLUR = strtof(optarg, NULL);
                printf ("set BLUR_ALPHA: `%f'\n", imageOptions.ALPHA_BLUR);
                break;

            case 'g':
                imageOptions.ALPHA_GRADIENT = strtof(optarg, NULL);
                printf ("set GRADIENT_ALPHA: `%f'\n", imageOptions.ALPHA_GRADIENT);
                break;

            case 'l':
                imageOptions.HYSTERESIS_THRESHOLD_LOW = (unsigned int) atoi(optarg);
                printf ("set HYSTERESIS_LOW: `%ud\n", imageOptions.HYSTERESIS_THRESHOLD_LOW);
                break;

            case 'h':
                imageOptions.HYSTERESIS_THRESHOLD_HIGH = (unsigned int) atoi(optarg);
                printf ("set HYSTERESIS_HIGH: `%ud\n", imageOptions.HYSTERESIS_THRESHOLD_HIGH);
                break;


            case '?':
                /* getopt_long already printed an error message. */
                break;

            default:
                abort();
        }
    }

    /* Instead of reporting ‘--verbose’
       and ‘--brief’ as they are encountered,
       we report the final status resulting from them. */
    if (VERBOSE)
        puts ("verbose flag is set");

    if (WRITE_CSV)
        puts ("write-csv flag is set");

    if (!OTSU_THRESHOLD)
        puts ("Otsu Threshold disabled");

    /* Print any remaining command line arguments (not options). */
    if (optind < argc)
    {
        while (optind < argc)
        {
            processImage(argv[optind++], &imageOptions);
        }
    }

//    processImage("./examples/image2.bmp", imageOptions);
//    processImage("./examples/image3.bmp", imageOptions);
//
//    /** Hollow Noisy (Handrawn) polygons **/
//    imageOptions.ALPHA_BLUR = 2;
//    imageOptions.ALPHA_GRADIENT = 0;
//    processImage("./examples/triangle.bmp", imageOptions);
//    processImage("./examples/hexagon.bmp", imageOptions);
//
//    /** Solid - Handrawn polygons **/
//    imageOptions.ALPHA_BLUR = 1;
//    imageOptions.ALPHA_GRADIENT = 10;
//    processImage("./examples/diamond.bmp", imageOptions);

    return 0;
}
