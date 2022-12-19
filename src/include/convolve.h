#pragma once

#include <stdlib.h>
#include <stdio.h>

int bound (float pixel);

unsigned char* convolve(unsigned char*image, int imgRows, int imgColumns, int rowStart, int rowEnd, int* kernel,int kRows, int kColumns) {

    int kRowCenter = kRows/2;
    int kColumnCenter = kColumns/2;

    unsigned char * partialConvolvedImage = malloc(sizeof(unsigned char)*(rowEnd-rowStart)*imgColumns);

    for (int i = rowStart; i < rowEnd; i++)
    {
        for (int j = 0; j < imgColumns; j++)
        {
            float newPixel = 0;
            for (int ki = 0; ki < kRows; ki++)
            {
                for (int kj = 0; kj < kColumns; kj++)
                {
                    //Center current pixel over the kernel.
                    int iCentered = i - kRowCenter + ki;
                    int jCentered = j - kColumnCenter + kj;

                    //Pad with zero.
                    if ((iCentered < 0|| iCentered >= imgRows) || (jCentered < 0 || jCentered >= imgColumns))
                    {
                        continue;
                    }

                    newPixel += image[iCentered*imgColumns + jCentered] * kernel[ki*kColumns+kj];
                }
            }

            partialConvolvedImage[(i-rowStart)*imgColumns + j] = bound(newPixel);
        }
    }


    return partialConvolvedImage;

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