#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#include "sobel.h"
#include "emboss.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../libs/stb/stb_image_write.h"

int main(int argc, char** argv) {

    int myRank, numOfProcs;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numOfProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    int imgColumns, imgRows, channels;
    int desiredChannels = 1;

    unsigned char* image1D;

    if (myRank == 0)
    {
        image1D = stbi_load("assets/1.png", &imgColumns, &imgRows, &channels, desiredChannels);
        if (image1D == NULL) {
            printf("Error while opening the image!\n");
            return 1;
        }

        printf("Image Info: imgColumns:%d, imgRows:%d, channels:%d\n", imgColumns, imgRows, channels);
    }

    MPI_Bcast(image1D, imgRows*imgColumns,MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);



    unsigned char* convolvedImage = sobel(image1D, imgRows, imgColumns);
    stbi_write_png("assets/out.png", imgColumns, imgRows, desiredChannels, convolvedImage, imgColumns * desiredChannels);
    // TODO: Check if output directory exists before saving the image.
    stbi_image_free(image1D);   
    free(convolvedImage);

    return 0;
}

