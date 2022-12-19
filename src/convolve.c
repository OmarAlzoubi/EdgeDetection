#include "convolve.h"
#include <stdlib.h>
#include <stdio.h>

int bound (float pixel);

unsigned char convolve(unsigned char*image, int imgRows, int imgColumns, unsigned char pixel, int row, int column, int* kernel,int kRows, int kColumns) {

    int kRowCenter = kRows/2;
    int kColumnCenter = kColumns/2;
    
    float newPixel = 0;
    for (int ki = 0; ki < kRows; ki++)
    {
        for (int kj = 0; kj < kColumns; kj++)
        {
            //Center current pixel over the kernel.
            int iCentered = row - kRowCenter + ki;
            int jCentered = column - kColumnCenter + kj;

            //Pad with zero.
            if ((iCentered < 0|| iCentered >= imgRows) || (jCentered < 0 || jCentered >= imgColumns))
            {
                continue;
            }

            newPixel += image[iCentered*imgColumns + jCentered] * kernel[ki*kColumns+kj];
        }
    }

    return bound(newPixel);

}

int bound (float pixel)
{
    if (pixel < 0)
    {
        pixel = 0;
    }
    else if (pixel > 255)
    {
        pixel = 255;
    }

    return (int)pixel;
}