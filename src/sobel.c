#include "convolve.h"
#include "kernels.h"
#include <stdlib.h>
#include <math.h>


unsigned char* combine (unsigned char* gx, unsigned char* gy, int imgRows, int imgColumns);

unsigned char* sobel (unsigned char* image, int imgRows, int imgColumns)
{
    unsigned char* gx = convolve(image, imgRows, imgColumns, sobelX[0], kRows, kColumns);
    unsigned char* gy = convolve(image, imgRows, imgColumns, sobelY[0], kRows, kColumns);
    unsigned char* edgeImage = combine(gx, gy, imgRows, imgColumns);

    free (gx);
    free (gy);

    return edgeImage;
}

unsigned char* combine (unsigned char* gx, unsigned char* gy, int imgRows, int imgColumns)
{
    unsigned char* image = malloc(sizeof(unsigned char) * imgRows * imgColumns);

    for (int i = 0; i < imgRows * imgColumns; i++)
    {
        int finalValue = hypot(gx[i], gy[i]);
        
        if (finalValue > 255)
        {
            finalValue = 255;
        }

        image[i] = finalValue;
    }

    return image;
}