#include "convolve.h"
#include "kernels.h"
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <stdio.h>
#include "task.h"

unsigned char combine (unsigned char gx, unsigned char gy);

void sobel (void* param)
{
    struct Task *task = (struct Task*)param;
    
    unsigned char* image = task->image;
    unsigned char* partialConvolvedImage = task->partialConvolvedImage;
    int imgRows = task->imgRows;
    int rowStart = task->rowStart;
    int imgColumns = task->imgColumns;
    int i = task->i;
    int j = task->j;

    unsigned char pixel = image[i*imgColumns+j];
    unsigned char gx = convolve(image, imgRows, imgColumns, pixel, i, j, sobelX[0], kRows, kColumns);
    unsigned char gy = convolve(image, imgRows, imgColumns, pixel, i, j, sobelY[0], kRows, kColumns);

    partialConvolvedImage[(i-rowStart)*imgColumns + j] = combine(gx, gy);

    free(task);
}

unsigned char combine (unsigned char gx, unsigned char gy)
{
    int finalValue = hypot(gx, gy);
    
    if (finalValue > 255)
    {
        finalValue = 255;
    }

    return finalValue;
}