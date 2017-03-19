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
} DericheCoeffs;

Mat dericheFilter(const Mat *image, const DericheCoeffs *coeffs) {

    const size_t height = image->height;
    const size_t width  = image->width;

    const size_t pixel_count = width * height;

    Mat y1 = createMatrix(image->width, image->height, 0);
    Mat y2 = createMatrix(image->width, image->height, 0);
    Mat out = createMatrix(image->width, image->height, 0);

    int x, y;
    for(y = 0; y < height; y++)
    {
        /** LEFT to RIGHT iteration **/
        for (x = 0; x < width; x++)
        {
            const size_t row = y * width;
            const size_t idx = row + x;
            const double xL_0 = image->data[idx];
            const double xL_1 = (x >= 1) ? image->data[idx - 1] : 0.0;
            const double yL_2 = (x >= 2) ? y1.data[idx - 2] : 0.0;
            const double yL_1 = (x >= 1) ? y1.data[idx - 1] : 0.0;
            y1.data[idx] = (coeffs->a1 * xL_0) + (coeffs->a2 * xL_1) + (coeffs->b1 * yL_1) + (coeffs->b2 * yL_2);
        }

        /** RIGHT to LEFT iteration **/
        for(x = (int)(width-1); x >= 0; --x)
        {
            const size_t row = y * width;
            const size_t idx = row + x;
            const double xR_2 = (width-x >= 2) ? image->data[idx + 2] : 0.0;
            const double xR_1 = (width-x >= 1) ? image->data[idx + 1] : 0.0;
            const double yR_2 = (width-x >= 2) ? y2.data[idx + 2] : 0.0;
            const double yR_1 = (width-x >= 1) ? y2.data[idx + 1] : 0.0;
            y2.data[idx] = (coeffs->a3 * xR_1) + (coeffs->a4 * xR_2) + (coeffs->b1 * yR_1) + (coeffs->b2 * yR_2);
        }

    }

    size_t k;
    for(k = 0; k < pixel_count; k++)
    {
        out.data[k] = (coeffs->c1) * (y1.data[k] + y2.data[k]);
    }


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
            y1.data[idx] = (coeffs->a1 * xL_0) + (coeffs->a2 * xL_1) + (coeffs->b1 * yL_1) + (coeffs->b2 * yL_2);
        }

        /** BOTTOM to TOP iteration **/
        for(y = (int)(height-1); y >= 0; --y)
        {
            const size_t row = y * width;
            const size_t idx = row + x;
            const double xR_2 = (height-y >= 2) ? image->data[idx + 2*width] : 0.0;
            const double xR_1 = (height-y >= 1) ? image->data[idx + 1*width] : 0.0;
            const double yR_2 = (height-y >= 2) ? y2.data[idx + 2*width] : 0.0;
            const double yR_1 = (height-y >= 1) ? y2.data[idx + 1*width] : 0.0;
            y2.data[idx] = (coeffs->a3 * xR_1) + (coeffs->a4 * xR_2) + (coeffs->b1 * yR_1) + (coeffs->b2 * yR_2);
        }

    }

    for(k = 0; k < pixel_count; k++)
    {
        out.data[k] = (coeffs->c2) * (y1.data[k] + y2.data[k]);
    }

    destroyMatrix(&y1);
    destroyMatrix(&y2);

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

            const double G_c  = hypot(Gradx->data[c], Grady->data[c]);
//            const double G_ee = hypot(Gradx->data[ee], Grady->data[ee]);
//            const double G_ww = hypot(Gradx->data[ww], Grady->data[ww]);
//            const double G_nw = hypot(Gradx->data[nw], Grady->data[nw]);
//            const double G_nn = hypot(Gradx->data[nn], Grady->data[nn]);
//            const double G_ne = hypot(Gradx->data[ne], Grady->data[ne]);
//            const double G_se = hypot(Gradx->data[se], Grady->data[se]);
//            const double G_ss = hypot(Gradx->data[ss], Grady->data[ss]);
//            const double G_sw = hypot(Gradx->data[sw], Grady->data[sw]);

            image->data[c] = (G_c >= MAGNITUDE_LIMIT) ? (MAGNITUDE_LIMIT) : 0.0;
            continue;
            const float dir = (float) (fmod(atan2(Grady->data[c], Gradx->data[c]) + M_PI, M_PI) / M_PI) * 8;
//
//            if (((dir <= 1 || dir > 7) && G_c > G_ee && G_c > G_ww) || // 0 deg
//                ((dir > 1 && dir <= 3) && G_c > G_nw && G_c > G_se) || // 45 deg
//                ((dir > 3 && dir <= 5) && G_c > G_nn && G_c > G_ss) || // 90 deg
//                ((dir > 5 && dir <= 7) && G_c > G_ne && G_c > G_sw))   // 135 deg
//
//                image->data[c] = G_c;
//            else
//                image->data[c] = 0.0;
//

        }
    }
//    size_t x,y;
//
//    for(x = 1; x < width-1; x++)
//    {
//        for (y = 1; y < height - 1; y++)
//        {
//            size_t idx = (y * width) + x;
//
//            int dx = (Gradx->data[idx] > 0) ? 1 : -1;
//            int dy = (Grady->data[idx] > 0) ? 1 : -1;
//
//            size_t a1_offset, b1_offset;
//
//            double first, second;
//
//            if (abs((int) Gradx->data[idx]) > abs((int) Grady->data[idx]))
//            {
//                a1_offset = (x + dx) + ((y + 00) * width);
//                b1_offset = (x - dx) + ((y + 00) * width);
//
//                first = Gradx->data[idx];
//                second = Grady->data[idx];
//            } else
//            {
//                a1_offset = (x + 00) + ((y - dy) * width);
//                b1_offset = (x + 00) + ((y + dy) * width);
//
//                first = Grady->data[idx];
//                second = Gradx->data[idx];
//            }
//
//            size_t a2_offset = (x + dx) + ((y - dy) * width);
//            size_t b2_offset = (x - dx) + ((y + dy) * width);
//
//            const double a1 = hypot(Gradx->data[a1_offset], Grady->data[a1_offset]);
//            const double a2 = hypot(Gradx->data[a2_offset], Grady->data[a2_offset]);
//            const double b1 = hypot(Gradx->data[b1_offset], Grady->data[b1_offset]);
//            const double b2 = hypot(Gradx->data[b2_offset], Grady->data[b2_offset]);
//
//            const double A = (first) - a1 * (second) + a2 * (second);
//            const double B = (first) - b1 * (second) + b2 * (second);
//
//            const double gradMag = hypot(Gradx->data[idx], Grady->data[idx]);
//            const double P = gradMag * (first);
//
//            double val = (P >= A && P >= B) ? gradMag : 0;
//
//            image->data[idx] = val;//0xff000000 | ((int) (val) << 16 | (int) (val) << 8 | (int) (val));
//            printf("graient:%f\n", image->data[idx]);
//            //(gradMag >= MAGNITUDE_LIMIT) ? (MAGNITUDE_LIMIT) : 0.0;
//        }
//    }
            //double a1, a2, b1, b2, A, B, point, val;
//
//            const double yGrad = Grady->data[k];
//            const double xGrad = Gradx->data[k];
//            const double gradMag = hypot(xGrad, yGrad);
//            const double gradOrientation = 2 * M_PI * atan2(yGrad, xGrad);
//            if(diffx[x + (y * width)] > 0) dx = 1;
//                    else dx = -1;
//
//                    if(diffy[x + (y * width)] > 0) dy = 1;
//                    else dy = -1;
//
//                    int a1, a2, b1, b2, A, B, point, val;
//                    if(Math.abs(diffx[x + (y * width)]) > Math.abs(diffy[x + (y * width)]))
//                    {
//                        a1 = mag[(x+dx) + ((y) * width)];
//                        a2 = mag[(x+dx) + ((y-dy) * width)];
//                        b1 = mag[(x-dx) + ((y) * width)];
//                        b2 = mag[(x-dx) + ((y+dy) * width)];
//                        A = (Math.abs(diffx[x + (y * width)]) - Math.abs(diffy[x + (y * width)]))*a1 + Math.abs(diffy[x + (y * width)])*a2;
//                        B = (Math.abs(diffx[x + (y * width)]) - Math.abs(diffy[x + (y * width)]))*b1 + Math.abs(diffy[x + (y * width)])*b2;
//                        point = mag[x + (y * width)] * Math.abs(diffx[x + (y * width)]);
//                        if(point >= A && point > B) {
//                            val = Math.abs(diffx[x + (y * width)]);
//                            output[x + (y * width)] = 0xff000000 | ((int)(val) << 16 | (int)(val ) << 8 | (int)(val));
//                        }
//                        else {
//                            val = 0;
//                            output[x + (y * width)] = 0xff000000 | ((int)(val) << 16 | (int)(val ) << 8 | (int)(val));
//                        }
//                    }
//                    else
//                    {
//                        a1 = mag[(x) + ((y-dy) * width)];
//                        a2 = mag[(x+dx) + ((y-dy) * width)];
//                        b1 = mag[(x) + ((y+dy) * width)];
//                        b2 = mag[(x-dx) + ((y+dy) * width)];
//                        A = (Math.abs(diffy[x + (y * width)]) - Math.abs(diffx[x + (y * width)]))*a1 + Math.abs(diffx[x + (y * width)])*a2;
//                        B = (Math.abs(diffy[x + (y * width)]) - Math.abs(diffx[x + (y * width)]))*b1 + Math.abs(diffx[x + (y * width)])*b2;
//                        point = mag[x + (y * width)] * Math.abs(diffy[x + (y * width)]);
//                        if(point >= A && point > B) {
//                            val = Math.abs(diffy[x + (y * width)]);
//                            output[x + (y * width)] = 0xff000000 | ((int)(val) << 16 | (int)(val ) << 8 | (int)(val));
//                        }
//                        else {
//                            val = 0;
//                            output[x + (y * width)] = 0xff000000 | ((int)(val) << 16 | (int)(val ) << 8 | (int)(val));
//                        }
//                    }
//                }
//            }
//        }
//    }
//
//    for(k = 0; k < pixel_count; k++)
//    {
//        const double yGrad = Grady->data[k];
//        const double xGrad = Gradx->data[k];
//
//        const double gradOrientation = 2 * M_PI * atan2(yGrad, xGrad);
//
//
//        image->data[k] = (gradMag >= MAGNITUDE_LIMIT) ? (MAGNITUDE_LIMIT) : 0.0;
//        //if(gradMag > 0.01)printf("ori:%f, mag: %f, x:%f, y:%f\n", gradOrientation, gradMag, xGrad, yGrad);
//        continue;
//        int indexN = k - width;
//        int indexS = k + width;
//        int indexW = k - 1;
//        int indexE = k + 1;
//        int indexNW = indexN - 1;
//        int indexNE = indexN + 1;
//        int indexSW = indexS - 1;
//        int indexSE = indexS + 1;
//
//
//        //perform non-maximal supression
//        const double nMag = hypot(Gradx->data[indexN], Grady->data[indexN]);
//        const double sMag = hypot(Gradx->data[indexS], Grady->data[indexS]);
//        const double wMag = hypot(Gradx->data[indexW], Grady->data[indexW]);
//        const double eMag = hypot(Gradx->data[indexE], Grady->data[indexE]);
//        const double neMag = hypot(Gradx->data[indexNE], Grady->data[indexNE]);
//        const double seMag = hypot(Gradx->data[indexSE], Grady->data[indexSE]);
//        const double swMag = hypot(Gradx->data[indexSW], Grady->data[indexSW]);
//        const double nwMag = hypot(Gradx->data[indexNW], Grady->data[indexNW]);
//
//        double tmp;
//        /*
//         * An explanation of what's happening here, for those who want
//         * to understand the source: This performs the "non-maximal
//         * supression" phase of the Canny edge detection in which we
//         * need to compare the gradient magnitude to that in the
//         * direction of the gradient; only if the value is a local
//         * maximum do we consider the point as an edge candidate.
//         *
//         * We need to break the comparison into a number of different
//         * cases depending on the gradient direction so that the
//         * appropriate values can be used. To avoid computing the
//         * gradient direction, we use two simple comparisons: first we
//         * check that the partial derivatives have the same sign (1)
//         * and then we check which is larger (2). As a consequence, we
//         * have reduced the problem to one of four identical cases that
//         * each test the central gradient magnitude against the values at
//         * two points with 'identical support'; what this means is that
//         * the geometry required to accurately interpolate the magnitude
//         * of gradient function at those points has an identical
//         * geometry (upto right-angled-rotation/reflection).
//         *
//         * When comparing the central gradient to the two interpolated
//         * values, we avoid performing any divisions by multiplying both
//         * sides of each inequality by the greater of the two partial
//         * derivatives. The common comparand is stored in a temporary
//         * variable (3) and reused in the mirror case (4).
//         *
//         */
//        //image->data[k] = (gradMag >= MAGNITUDE_LIMIT) ? (1000.0) : ((100.0 * gradMag));
//        if (xGrad * yGrad <= (double) 0 /*(1)*/
//            ? abs(xGrad) >= abs(yGrad) /*(2)*/
//            ? (tmp = abs(xGrad * gradMag)) >= abs(yGrad * neMag - (xGrad + yGrad) * eMag) /*(3)*/
//                && tmp > abs(yGrad * swMag - (xGrad + yGrad) * wMag) /*(4)*/
//              : (tmp = abs(yGrad * gradMag)) >= abs(xGrad * neMag - (yGrad + xGrad) * nMag) /*(3)*/
//                && tmp > abs(xGrad * swMag - (yGrad + xGrad) * sMag) /*(4)*/
//            : abs(xGrad) >= abs(yGrad) /*(2)*/
//              ? (tmp = abs(xGrad * gradMag)) >= abs(yGrad * seMag + (xGrad - yGrad) * eMag) /*(3)*/
//                && tmp > abs(yGrad * nwMag + (xGrad - yGrad) * wMag) /*(4)*/
//              : (tmp = abs(yGrad * gradMag)) >= abs(xGrad * seMag + (yGrad - xGrad) * sMag) /*(3)*/
//                && tmp > abs(xGrad * nwMag + (yGrad - xGrad) * nMag) /*(4)*/
//                ) {
//
//            image->data[k] = (gradMag >= MAGNITUDE_LIMIT) ? (1000.0) : ((1000.0 * gradMag));
//            //NOTE: The orientation of the edge is not employed by this
//            //implementation. It is a simple matter to compute it at
//            //this point as: Math.atan2(yGrad, xGrad);
//        } else {
//            image->data[k] = 0.0;
//        }
//    }
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

Mat dericheBlur(const Mat *image, const double ALPHA) {

    const double k_numerator = (1.0 - exp(-1.0 * ALPHA)) * (1.0 - exp(-1.0 * ALPHA));
    const double k_denominator = 1.0 + (2.0 * ALPHA * exp(-1.0 * ALPHA)) - exp(-2.0 * ALPHA);
    const double k = k_numerator/k_denominator;

    /** Prepare co-efficients for blurring pass **/
    DericheCoeffs blurCoeffs;
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

    /** perform smoooting  **/
    Mat blurred = dericheFilter(image, &blurCoeffs);
    return blurred;
}

void edgeDetect(Mat *image, const double ALPHA, const int LOW_HYS, const int HIGH_HYS, const int BLUR) {

    const double k_numerator = (1.0 - exp(-1.0 * ALPHA)) * (1.0 - exp(-1.0 * ALPHA));
    const double k_denominator = 1.0 + (2.0 * ALPHA * exp(-1.0 * ALPHA)) - exp(-2.0 * ALPHA);
    const double k = k_numerator/k_denominator;

    /** Prepare co-efficients for blurring pass **/
    DericheCoeffs blurCoeffs;
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

    if(BLUR)
    {
        /** perform smoooting  **/
        dericheFilter(image, &blurCoeffs);
    }
    /** Prepare co-efficients for x-derivative pass **/
    DericheCoeffs xDerivativeCoeffs;
    xDerivativeCoeffs.a1 = 0.0;
    xDerivativeCoeffs.a2 = 1.0;
    xDerivativeCoeffs.a3 = -1.0;
    xDerivativeCoeffs.a4 = 0.0;
    xDerivativeCoeffs.a5 = blurCoeffs.a1;
    xDerivativeCoeffs.a6 = blurCoeffs.a2;
    xDerivativeCoeffs.a7 = blurCoeffs.a3;
    xDerivativeCoeffs.a8 = blurCoeffs.a4;
    xDerivativeCoeffs.b1 = blurCoeffs.b1;
    xDerivativeCoeffs.b2 = blurCoeffs.b2;
    xDerivativeCoeffs.c1 = -1.0 * k_numerator;
    xDerivativeCoeffs.c2 = 1.0;


    /** perform x-derivative **/
    Mat xGradient = dericheFilter(image, &xDerivativeCoeffs);

    /** Prepare co-efficients for x-derivative pass **/
    DericheCoeffs yDerivativeCoeffs;
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

    /** perform x-derivative **/
    Mat yGradient = dericheFilter(image, &yDerivativeCoeffs);

    getGradient(&xGradient, &yGradient, image);

    if(LOW_HYS >= 1 && HIGH_HYS >=1) hysteresisThreshold(image, LOW_HYS, HIGH_HYS);

    destroyMatrix(&yGradient);
    destroyMatrix(&xGradient);
}


#endif //HOUGHTRANSFORM_DERICHE_H
