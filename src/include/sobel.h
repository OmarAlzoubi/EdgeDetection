#pragma once
#include "convolve.h"
#include "kernels.h"
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <stdio.h>
#include "threadArgs.h"

unsigned char* combine (unsigned char* gx, unsigned char* gy, int imgRows, int imgColumns);

void sobel (void* param)
{
    struct ThreadArgs *task = (struct ThreadArgs*)param;

    unsigned char* image = task->image;
    unsigned char** partialConvolvedImage = task->partialConvolvedImage;
    int imgRows = task->imgRows;
    int rowStart = task->rowStart;
    int rowEnd = task->rowEnd;
    int imgColumns = task->imgColumns;
    unsigned char* gx = convolve(image, imgRows, imgColumns, rowStart, rowEnd, sobelX[0], kRows, kColumns);
    unsigned char* gy = convolve(image, imgRows, imgColumns, rowStart, rowEnd, sobelY[0], kRows, kColumns);

    *partialConvolvedImage = combine(gx, gy, rowEnd-rowStart, imgColumns);
    free(gx);
    free(gy);
    free(task);

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