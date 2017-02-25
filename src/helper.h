//
// Created by Work Account on 2/23/17.
//

#ifndef HOUGHTRANSFORM_HELPER_H
#define HOUGHTRANSFORM_HELPER_H


typedef struct
{
    size_t width;
    size_t height;
    double *data;
} Mat;

void writeMat_toCSV(char* name, Mat imageMap) {

    FILE *MATout = fopen(name, "w");

    if(MATout == NULL)
    {
        perror("Unable to write file, exiting");
        return;
    }

    const size_t w = imageMap.width;
    const size_t h = imageMap.height;

    size_t x, y;

    for(y=0; y < h; y++)
    {
        for(x=0; x < (w-1); x++)
        {
            fprintf(MATout, "%f,", imageMap.data[(y*w) + x]);
        }
        fprintf(MATout, "%f\n", imageMap.data[(y * (w)) + (w-1)]);
    }

}

Mat createMatrix(const size_t width, const size_t height, const unsigned int zeroed) {

    Mat newMatrix;
    newMatrix.width = width;
    newMatrix.height = height;

    const size_t pixel_count = width * height;

    newMatrix.data = malloc(pixel_count * sizeof(double));

    if (newMatrix.data == NULL)
    {
        printf("memory asked :%lu", pixel_count * sizeof(double));
        perror("unable to allocate memory for matrix. exiting.");
        exit(1);
    }

    /* Set matrix values to zero */
    if(!zeroed)
    {
        size_t i;
        for (i = 0; i < pixel_count; i++)
        {
            newMatrix.data[i] = 0.0;
        }
    }

    return newMatrix;
}

void destroyMatrix(Mat *toDestroy) {
    free(toDestroy->data);
}

#endif //HOUGHTRANSFORM_HELPER_H
