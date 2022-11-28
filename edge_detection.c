#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "libs/stb/stb_image_write.h"

int const kRows = 3;
int const kColumns = 3;

int imgRows;
int imgColumns;
int channels;
int const desiredChannels = 1;

unsigned char* convolve(unsigned char*image, int* kx, int* ky);


unsigned char* edgeDetect(unsigned char* image1D, int* kx, int* ky) {

    unsigned char* edgeImage = convolve(image1D, kx, ky);
    return edgeImage;
}

int main() {
    
    //Kernel
    int* kx = malloc(sizeof(int)*kRows*kColumns);
    kx[0] = 1; kx[1] = 0; kx[2] = -1;
    kx[3] = 2; kx[4] = 0; kx[5] = -2;
    kx[6] = 1; kx[7] = 0; kx[8] = -1;
    
    int* ky = malloc(sizeof(int)*kRows*kColumns);
    ky[0] = 1;  ky[1] = 2;   ky[2] = 1;
    ky[3] = 0;  ky[4] = 0;   ky[5] = 0;
    ky[6] = -1; ky[7] = -2; ky[8] = -1;

    unsigned char* image1D = stbi_load("assets/4.png", &imgColumns, &imgRows, &channels, desiredChannels);
    if (image1D == NULL) {
        printf("Error while opening the image!\n");
        return 1;
    }

    printf("Image Info: imgColumns:%d, imgRows:%d, channels:%d\n", imgColumns, imgRows, channels);

    unsigned char* edgeImage= edgeDetect(image1D, kx, ky);
    stbi_write_png("assets/edgeImage.png", imgColumns, imgRows, desiredChannels, edgeImage, imgColumns * desiredChannels);

    stbi_image_free(image1D);
    free(edgeImage);
    free(kx);
    free(ky);

    return 0;
}


unsigned char* convolve(unsigned char*image, int* kx, int* ky) {

    unsigned char* edgeImage = malloc(sizeof(unsigned char)*imgRows*imgColumns);

    int kRowCenter = kRows/2;
    int kColumnCenter = kColumns/2;
    for (int i = 0; i < imgRows*imgColumns; i++) {

        int gx = 0;
        int gy = 0;

        for (int ki = 0; ki < kRows; ki++) {

            //Center the kernel over pixel[i][j]
            int currentRow = i/imgRows - kRowCenter + ki;

            //No ovelap with kernel -> Pad with zero.
            if ((currentRow < 0) || (currentRow > imgRows - 1)){
                continue;
            }

            for (int kj = 0; kj < kColumns; kj++){

                //Center the kernel over pixel[i][j]
                int currentColumn = i%imgColumns - kColumnCenter + kj;

                //No ovelap with kernel -> Pad with zero.
                if ((currentColumn < 0) || (currentColumn > imgColumns -1)){
                    continue;
                }

                //Therse if overlap.

                int imageIDIndex = currentRow*imgColumns + currentColumn;
                int kernelIDIndex = ki*kColumns + kj;

                //printf("Here:%d\n", image[imageIDIndex]);
                gx += image[imageIDIndex]*kx[kernelIDIndex];
                gy += image[imageIDIndex]*ky[kernelIDIndex];
                
            }
        }

        int newPixel = sqrt(pow(gx, 2) + pow(gy, 2));

        if (newPixel < 0) {
            newPixel = 0;
        }else if (newPixel > 255) {
            newPixel = 255;
        }
          
        edgeImage[i] = newPixel;


        
    }

    return edgeImage;
}