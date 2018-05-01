//
// Created by Arjun on 4/30/18.
//

#include "include/otsu.h"

unsigned int computeThreshold(Mat* image) {

    const size_t pixels = image->width * image->height;

    size_t *histogram  = calloc(256, sizeof(size_t));

    if(histogram == NULL)
        exit(1);

    normalizeImage(image);

    size_t i;
    for(i = 0; i < pixels; i++)
    {
        unsigned char h = 0xFF & (unsigned char)(roundf(image->data[i]));
        histogram[h]++;
    }

    float sum = 0;
    for (i = 0; i<256; i++)
    {
        sum += i * histogram[i];
    }

    float sumB = 0;
    size_t wB = 0;
    size_t wF = 0;

    float varMax = 0;
    unsigned char threshold = 0;

    unsigned int t;
    for (t = 0 ; t < 256 ; t++)
    {
        wB += histogram[t];               // Weight Background
        if (wB == 0)
            continue;

        wF = pixels - wB;                 // Weight Foreground
        if (wF == 0)
            break;

        sumB += (float) (t * histogram[t]);

        float mB = sumB / wB;            // Mean Background
        float mF = (sum - sumB) / wF;    // Mean Foreground

        // Calculate Between Class Variance
        float varBetween = (float) wB * (float) wF * (mB - mF) * (mB - mF);

        // Check if new maximum found
        if (varBetween > varMax)
        {
            varMax = varBetween;
            threshold = (unsigned char) t;
        }
    }


    free(histogram);

    if(threshold > 0)
    {
        for(i = 0; i < pixels; i++)
        {
            image->data[i] = (image->data[i] > threshold) ? 255 : 0;
        }
    }


    return threshold;
}
