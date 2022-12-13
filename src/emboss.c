#include "emboss.h"
#include "convolve.h"
#include "kernels.h"
#include <stdlib.h>


unsigned char* blur (unsigned char* image, int imgRows, int imgColumns, int* kernel, int kRows, int kColumns)
{
    unsigned char* blurredImage = convolve(image, imgRows, imgColumns, emboss[0], kRows, kColumns);

    return blurredImage;    
}