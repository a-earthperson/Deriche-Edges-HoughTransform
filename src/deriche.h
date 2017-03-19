//
// Created by Work Account on 2/23/17.
//

#ifndef HOUGHTRANSFORM_DERICHE_H
#define HOUGHTRANSFORM_DERICHE_H

#include "hough.h"
#include "helper.h"

#define MAGNITUDE_LIMIT 255.0

typedef struct {
    double a1;
    double a2;
    double a3;
    double a4;
    double a5;
    double a6;
    double a7;
    double a8;
    double b1;
    double b2;
    double c1;
    double c2;
} Coeffs;

typedef struct {
    Coeffs blur;
    Coeffs xGradient;
    Coeffs yGradient;
} DericheCoeffs;


Mat applyDericheFilter(Mat *image, const Coeffs *coeffs) {

    const int height = (int)(image->height);
    const int width  = (int)(image->width);

    const int pixel_count = width * height;

    Mat y1  = createMatrix(image->width, image->height, 0);
    Mat y2  = createMatrix(image->width, image->height, 0);
    Mat out = createMatrix(image->width, image->height, 0);

    int x, y;
    for(x = 0; x < width; x++)
    {
        /** TOP to BOTTOM iteration **/
        for (y = 0; y < height; y++)
        {
            const size_t row = y * width;
            const size_t idx = row + x;
            const double xL_1 = (y >= 1) ? image->data[idx - 1*width] : 0.0;
            const double xL_0 = image->data[idx];
            const double yL_2 = (y >= 2) ? y1.data[idx - 2*width] : 0.0;
            const double yL_1 = (y >= 1) ? y1.data[idx - 1*width] : 0.0;
            y1.data[idx] = fma(coeffs->a1, xL_0, fma(coeffs->a2, xL_1, fma(coeffs->b1, yL_1, fma(coeffs->b2, yL_2, 0))));
        }

        /** BOTTOM to TOP iteration **/
        for(y = (height-1); y >= 0; --y)
        {
            const size_t row = y * width;
            const size_t idx = row + x;
            const double xR_2 = (height-y >= 2) ? image->data[idx + 2*width] : 0.0;
            const double xR_1 = (height-y >= 1) ? image->data[idx + 1*width] : 0.0;
            const double yR_2 = (height-y >= 2) ? y2.data[idx + 2*width] : 0.0;
            const double yR_1 = (height-y >= 1) ? y2.data[idx + 1*width] : 0.0;
            y2.data[idx] =  fma(coeffs->a3, xR_1, fma(coeffs->a4, xR_2, fma(coeffs->b1, yR_1, fma(coeffs->b2, yR_2, 0))));
        }

    }

    int k;
    for(k = 0; k < pixel_count; k++)
    {
        out.data[k] = fma(coeffs->c1, (y1.data[k] + y2.data[k]), 0);  //(coeffs->c1) * (y1.data[k] + y2.data[k]);
    }

    for(y = 0; y < height; y++)
    {
        /** LEFT to RIGHT iteration **/
        for (x = 0; x < width; x++)
        {
            const size_t row  = y * width;
            const size_t idx  = row + x;
            const double xL_0 = image->data[idx];
            const double xL_1 = (x >= 1) ? image->data[idx - 1] : 0.0;
            const double yL_2 = (x >= 2) ? y1.data[idx - 2]     : 0.0;
            const double yL_1 = (x >= 1) ? y1.data[idx - 1]     : 0.0;
            y1.data[idx] = fma(coeffs->a1, xL_0, fma(coeffs->a2, xL_1, fma(coeffs->b1, yL_1, fma(coeffs->b2, yL_2, 0))));
        }

        /** RIGHT to LEFT iteration **/
        for(x = (width-1); x >= 0; --x)
        {
            const size_t row  = y * width;
            const size_t idx  = row + x;
            const double xR_2 = (width-x >= 2) ? image->data[idx + 2] : 0.0;
            const double xR_1 = (width-x >= 1) ? image->data[idx + 1] : 0.0;
            const double yR_2 = (width-x >= 2) ? y2.data[idx + 2]     : 0.0;
            const double yR_1 = (width-x >= 1) ? y2.data[idx + 1]     : 0.0;
            y2.data[idx] =  fma(coeffs->a3, xR_1, fma(coeffs->a4, xR_2, fma(coeffs->b1, yR_1, fma(coeffs->b2, yR_2, 0))));
        }

    }

    for(k = 0; k < pixel_count; k++)
    {
        out.data[k] = hypot(fma(coeffs->c2, (y1.data[k] + y2.data[k]), 0), out.data[k]);
        //out.data[k] = fma(coeffs->c2, (y1.data[k] + y2.data[k]), out.data[k]);//(coeffs->c2) * (y1.data[k] + y2.data[k]);
    }

    destroyMatrix(&y1);
    destroyMatrix(&y2);

    return out;
}

GradMat getGradientMat(Mat *Gradx, Mat *Grady) {

    GradMat out = createGradMatrix(Gradx->width, Gradx->height);

    const int height = (int)(Gradx->height);
    const int width  = (int)(Gradx->width);
    const int pixel_count = width * height;

    int k;
    for(k = 0; k < pixel_count; k++)
    {
        out.mag[k] = hypot(Gradx->data[k],Grady->data[k]);
        out.dir[k] = atan2(Gradx->data[k],Grady->data[k]);
    }

    return out;
}

void getGradient(Mat *Gradx, Mat *Grady, Mat*image) {

    if(Gradx->height != Grady->height || Gradx->width != Grady->width)
    {
        perror("cannot compare matrices of different sizes! exiting getGradient");
        exit(1);
    }
    const size_t height = Grady->height;
    const size_t width  = Grady->width;

    const size_t pixel_count = width * height;

    int prev_gradient_direction_quadrant = -1;

    for (int i = 1; i < width - 1; i++)
    {
        for (int j = 1; j < height - 1; j++)
        {
            const int c = i + width * j;
            const int nn = c - width;
            const int ss = c + width;
            const int ww = c + 1;
            const int ee = c - 1;
            const int nw = nn + 1;
            const int ne = nn - 1;
            const int sw = ss + 1;
            const int se = ss - 1;

            const double G_c = hypot(Gradx->data[c], Grady->data[c]);
            //const double G_d = atan2(Gradx->data[c], Grady->data[c]);

            //int direction_quadrant;
//
//            if(G_d >= 0.0 && G_d <= M_PI_2)
//                direction_quadrant = 0;
//
//            else if(G_d >= M_PI_2  && G_d <= M_PI)
//                direction_quadrant = 1;
//
//            else if(G_d >= M_PI  && G_d <= (M_PI + M_PI_2))
//                direction_quadrant = 2;
//
//            else if(G_d >= M_PI  && G_d <= (M_PI + M_PI_2))
//                direction_quadrant = 3;

            //printf("%lu :: hypot: %f, atan2: %f, direction: %lu, prev:%lu\n", c, G_c, G_d, direction_quadrant, prev_gradient_direction_quadrant);
            //const double G_ee = hypot(Gradx->data[ee], Grady->data[ee]);

            image->data[c] = ((G_c >= MAGNITUDE_LIMIT)) ? MAGNITUDE_LIMIT : 0.0;//(G_c >= MAGNITUDE_LIMIT) ? (MAGNITUDE_LIMIT) : 0.0;

           // prev_gradient_direction_quadrant = direction_quadrant;


            //printf("%lu :: hypot: %f, direction: %f\n", c, G_c, G_d);
            const double G_ee = hypot(Gradx->data[ee], Grady->data[ee]);
            const double G_ww = hypot(Gradx->data[ww], Grady->data[ww]);
            const double G_nw = hypot(Gradx->data[nw], Grady->data[nw]);
            const double G_nn = hypot(Gradx->data[nn], Grady->data[nn]);
            const double G_ne = hypot(Gradx->data[ne], Grady->data[ne]);
            const double G_se = hypot(Gradx->data[se], Grady->data[se]);
            const double G_ss = hypot(Gradx->data[ss], Grady->data[ss]);
            const double G_sw = hypot(Gradx->data[sw], Grady->data[sw]);

            //image->data[c] = (G_c >= MAGNITUDE_LIMIT) ? (MAGNITUDE_LIMIT) : 0.0;
          //  continue;
            const float dir = (float) (fmod(atan2(Grady->data[c], Gradx->data[c]) + M_PI, M_PI) / M_PI) * 8;

//            if(G_c >= MAGNITUDE_LIMIT)
//            {
//                image->data[c] = MAGNITUDE_LIMIT;
//            }
            if (G_c >= MAGNITUDE_LIMIT)
            {
                if (((dir <= 1 || dir > 7) && G_c > G_ee && G_c > G_ww) || // 0 deg
                    ((dir > 1 && dir <= 3) && G_c > G_nw && G_c > G_se) || // 45 deg
                    ((dir > 3 && dir <= 5) && G_c > G_nn && G_c > G_ss) || // 90 deg
                    ((dir > 5 && dir <= 7) && G_c > G_ne && G_c > G_sw))   // 135 deg

                    image->data[c] = G_c;//MAGNITUDE_LIMIT;
                else
                    image->data[c] = 0.0;
            }
            else
                image->data[c] = 0.0;
        }
    }
}

void hysteresisConnect(Mat *image, const size_t x, const size_t y, const int low) {

    const size_t width = image->width;
    const size_t height = image->height;

    size_t xx, yy;

    for (xx = x - 1; xx <= x + 1; xx++)
    {
        for (yy = y - 1; yy <= y + 1; yy++)
        {
            if ((xx < width) && (yy < height) && (xx >= 0) & (yy >= 0) & (xx != x) & (yy != y))
            {
                const size_t idx = (yy * width) + xx;
                const double pxl_val = image->data[idx] ;//& 0xff;

                if (pxl_val != 255.0)
                {
                    if (pxl_val >= low)
                    {
                        image->data[idx] = 255.0;
                        hysteresisConnect(image, xx, yy, low);
                    }
                    else
                    {
                        image->data[idx] = 0.0;
                    }
                }
            }
        }
    }

}
void hysteresisThreshold(Mat *image, const int low, const int high) {

    const size_t width  = image->width;
    const size_t height = image->height;

    size_t x,y;
    for(x = 1; x < width; x++)
    {
        for (y = 1; y < height; y++)
        {
            const size_t idx = (y * width) + x;
            const double pxl = image->data[idx];

            if(pxl >= high)
            {
                image->data[idx] = 255.0;
                hysteresisConnect(image, x, y, low);
            }
        }
    }

    for(x = 0; x < width; x++)
    {
        for (y = 0; y < height; y++)
        {
            const size_t idx = (y * width) + x;
            const double pxl = image->data[idx];

            if(pxl != 255.0)
            {
                image->data[idx] = 0.0;
            }
        }
    }
}


/**
 * Generates deriche co-efficients from alpha
 * @param alpha
 * @return
 */
DericheCoeffs getDericheCoeffs(const double ALPHA) {

    const double k = (cosh(ALPHA) - 1.0f)/(ALPHA + sinh(ALPHA));

    /** Prepare co-efficients for blurring pass **/
    Coeffs blurCoeffs;
    blurCoeffs.a1 = k;
    blurCoeffs.a2 = k * exp(-1.0 * ALPHA) * (ALPHA - 1.0);
    blurCoeffs.a3 = k * exp(-1.0 * ALPHA) * (ALPHA + 1.0);
    blurCoeffs.a4 = -1.0 * k * exp(-2.0 * ALPHA);
    blurCoeffs.a5 = blurCoeffs.a1;
    blurCoeffs.a6 = blurCoeffs.a2;
    blurCoeffs.a7 = blurCoeffs.a3;
    blurCoeffs.a8 = blurCoeffs.a4;
    blurCoeffs.b1 = 2.0 * exp(-1.0 * ALPHA);
    blurCoeffs.b2 = -1.0 * exp(-2.0 * ALPHA);
    blurCoeffs.c1 = 1.0;
    blurCoeffs.c2 = 1.0;

    /** Prepare co-efficients for x-derivative pass **/
    Coeffs xDerivativeCoeffs;
    xDerivativeCoeffs.a1 =  0.0;
    xDerivativeCoeffs.a2 =  1.0;
    xDerivativeCoeffs.a3 = -1.0;
    xDerivativeCoeffs.a4 =  0.0;
    xDerivativeCoeffs.a5 = blurCoeffs.a1;
    xDerivativeCoeffs.a6 = blurCoeffs.a2;
    xDerivativeCoeffs.a7 = blurCoeffs.a3;
    xDerivativeCoeffs.a8 = blurCoeffs.a4;
    xDerivativeCoeffs.b1 = blurCoeffs.b1;
    xDerivativeCoeffs.b2 = blurCoeffs.b2;
    xDerivativeCoeffs.c1 = -1.0 * (1.0 - exp(-1.0 * ALPHA)) * (1.0 - exp(-1.0 * ALPHA));
    xDerivativeCoeffs.c2 =  1.0;

    /** Prepare co-efficients for y-derivative pass **/
    Coeffs yDerivativeCoeffs;
    yDerivativeCoeffs.a1 = xDerivativeCoeffs.a5;
    yDerivativeCoeffs.a2 = xDerivativeCoeffs.a6;
    yDerivativeCoeffs.a3 = xDerivativeCoeffs.a7;
    yDerivativeCoeffs.a4 = xDerivativeCoeffs.a8;
    yDerivativeCoeffs.a5 = xDerivativeCoeffs.a1;
    yDerivativeCoeffs.a6 = xDerivativeCoeffs.a2;
    yDerivativeCoeffs.a7 = xDerivativeCoeffs.a3;
    yDerivativeCoeffs.a8 = xDerivativeCoeffs.a4;
    yDerivativeCoeffs.b1 = xDerivativeCoeffs.b1;
    yDerivativeCoeffs.b2 = xDerivativeCoeffs.b2;
    yDerivativeCoeffs.c1 = xDerivativeCoeffs.c2;
    yDerivativeCoeffs.c2 = xDerivativeCoeffs.c1;

    DericheCoeffs coeffs;
    coeffs.blur = blurCoeffs;
    coeffs.xGradient = xDerivativeCoeffs;
    coeffs.yGradient = yDerivativeCoeffs;

    return coeffs;
}

Mat edgeDetect(Mat *image, const Options filterOptions) {

    Mat blurredImage = *image;

    /** Apply blur if option is set **/
    if (filterOptions.ALPHA_BLUR > 0)
    {
        const DericheCoeffs dericheCoeffs = getDericheCoeffs(filterOptions.ALPHA_BLUR);
        blurredImage = applyDericheFilter(image, &dericheCoeffs.blur);
    }

    /** Perform hysteresis thresholding **/
    if(filterOptions.HYSTERESIS_THRESHOLD_LOW > 0 && filterOptions.HYSTERESIS_THRESHOLD_HIGH > 0)
    {
        hysteresisThreshold(&blurredImage, filterOptions.HYSTERESIS_THRESHOLD_LOW, filterOptions.HYSTERESIS_THRESHOLD_HIGH);
    }

    /** No gradient related operations will be performed **/
    if(filterOptions.ALPHA_GRADIENT <= 0)
    {
        /** since blurred image will be returned, free image memory and return **/
        if (filterOptions.ALPHA_BLUR > 0)
        {
            destroyMatrix(image);
        }
        return blurredImage;
    }

    /** Perform all gradient analysis including X, Y gradient & magnitude supression **/

    const DericheCoeffs dericheCoeffs = getDericheCoeffs(filterOptions.ALPHA_GRADIENT);
    Mat xGradient = applyDericheFilter(&blurredImage, &dericheCoeffs.xGradient);
    Mat yGradient = applyDericheFilter(&blurredImage, &dericheCoeffs.yGradient);

    getGradient(&xGradient, &yGradient, &blurredImage);

    destroyMatrix(&yGradient);
    destroyMatrix(&xGradient);

    if (filterOptions.ALPHA_BLUR > 0)
    {
        destroyMatrix(image);
    }

    return blurredImage;
}


#endif //HOUGHTRANSFORM_DERICHE_H
