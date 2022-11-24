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

int** allocate2DImage();
void deallocate2DImage(int** array);
int** convolve(int **image, int kx[kRows][kColumns], int ky[kRows][kColumns]);
void map1Dto2D(unsigned char* image1D, int** image2D);
void map2Dto1D(int** image2D, unsigned char* image1D);

void edgeDetect(unsigned char* image1D, int** image2D,int kx[kRows][kColumns], int ky[kRows][kColumns]) {

    image2D = convolve(image2D, kx, ky);

    //Write the changes back to the image.
    map2Dto1D(image2D, image1D);
    deallocate2DImage(image2D);

}

int main() {
    
    //Kernel
    int kx[3][3] = {{1, 0, -1},{2, 0, -2},{1, 0, -1}};
    int ky[3][3] = {{1, 2, 1},{0, 0, 0},{-1, -2, -1}};

    unsigned char *image1D = stbi_load("assets/3.png", &imgColumns, &imgRows, &channels, desiredChannels);
    if (image1D == NULL) {
        printf("Error while opening the image!\n");
        return 1;
    }

    printf("Image Info: imgColumns:%d, imgRows:%d, channels:%d\n", imgColumns, imgRows, channels);

    int** image2D = allocate2DImage();
    map1Dto2D(image1D, image2D);
     
    edgeDetect(image1D, image2D, kx, ky);

    stbi_write_png("assets/edgeImage.png", imgColumns, imgRows, desiredChannels, image1D, imgColumns * desiredChannels);
    stbi_image_free(image1D);

    return 0;
}

int** allocate2DImage(){

    int** array = malloc(sizeof(int*)*imgRows);
    for (int i = 0; i < imgRows; i++){
        array[i] = malloc(sizeof(int) * imgColumns);
    }

    return array;
}

void deallocate2DImage(int** image2D) {

    for (int i = 0; i < imgRows; i++){
        free(image2D[i]);
    }

    free(image2D);    
}

void map1Dto2D(unsigned char* image1D, int** image2D){

    int outerIndex = 0;
    for (int i = 0; i < imgRows; i++) {
        for(int j = 0; j < imgColumns; j++){
            image2D[i][j] = image1D[outerIndex];
            outerIndex++;
        }
    }
}

void map2Dto1D(int** image2D, unsigned char* image1D){

    int outerIndex = 0;
    for (int i = 0; i < imgRows; i++) {
        for (int j = 0; j < imgColumns; j++) {
            image1D[outerIndex] = image2D[i][j];
            outerIndex++;
        }
    }
}

int** convolve(int **image, int kx[kRows][kColumns], int ky[kRows][kColumns]) {

    int **edgeImage = allocate2DImage();

    for (int i = 0; i < imgRows; i++) {
        for (int j = 0; j < imgColumns; j++){
            int gx = 0;
            int gy = 0;

            for (int ki = 0; ki < kRows; ki++) {
                for (int kj = 0; kj < kColumns; kj++){

                    int syncI = i - (kRows/2) + ki;
                    int syncJ = j - (kRows/2) + kj;

                    //Padd with zero

                    if ((syncI < 0) || (syncI > (imgRows-1))){
                        continue;
                    }

                    if ((syncJ < 0) || (syncJ > (imgColumns-1))){
                        continue;
                    }

                    gx = gx + image[syncI][syncJ] * kx[ki][kj];
                    gy = gy + image[syncI][syncJ] * ky[ki][kj];
                }
            }

            int newPixel = sqrt(pow(gx, 2) + pow(gy, 2));            
            edgeImage[i][j] = newPixel;
        }


    }

    deallocate2DImage(image);
    return edgeImage;
}