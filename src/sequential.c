#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "sobel.h"
#include "emboss.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../libs/stb/stb_image_write.h"

int main(int argc, char** argv) {

    char* inputPath;
    char* outputPath;
    if (argc != 3)
    {
        printf("Wrong number of argumnets!\n");
        return 1;
    }
    else
    {
        inputPath = argv[1];
        outputPath = argv[2];
    }

    int imgColumns, imgRows, channels;
    int desiredChannels = 1;

    unsigned char* image1D = stbi_load(inputPath, &imgColumns, &imgRows, &channels, desiredChannels);
    if (image1D == NULL) {
        printf("Error while opening the image!\n");
        return 1;
    }

    printf("Image Info: imgColumns:%d, imgRows:%d, channels:%d\n", imgColumns, imgRows, channels);

    unsigned char* convolvedImage = sobel(image1D, imgRows, imgColumns);
    stbi_write_png(strcat(outputPath,"OUT.png"), imgColumns, imgRows, desiredChannels, convolvedImage, imgColumns * desiredChannels);
    // TODO: Check if output directory exists before saving the image.
    stbi_image_free(image1D);   
    free(convolvedImage);

    return 0;
}

