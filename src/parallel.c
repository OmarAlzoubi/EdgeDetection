#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <pthread.h>

#include "../libs/C-Thread-Pool/thpool.h"
#include "sobel.h"
#include "task.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../libs/stb/stb_image_write.h"

int main(int argc, char** argv) {

    int myRank, numOfProcs;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numOfProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    unsigned char* image1D;
    unsigned char* partialConvolvedImage;
    unsigned char* convolvedImage;

    int MASTER = numOfProcs - 1;

    int imgColumns, imgRows, channels;
    int desiredChannels = 1;

    //For MPI_Gatherv.
    int *displs,*recivedCount;

    //Read image by master proc.
    if (myRank == MASTER)
    {
        image1D = stbi_load("assets/9.jpg", &imgColumns, &imgRows, &channels, desiredChannels);
        if (image1D == NULL) {
            printf("Error while opening the image!\n");
            return 1;
        }

        printf("Image Info: imgColumns:%d, imgRows:%d, channels:%d\n", imgColumns, imgRows, channels);

        //Allocate array for output image.
        convolvedImage = (unsigned char*)malloc(sizeof(unsigned char) * imgRows * imgColumns);
    }

    MPI_Bcast(&imgRows, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
    MPI_Bcast(&imgColumns, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
    MPI_Bcast(&channels, 1, MPI_INT, MASTER, MPI_COMM_WORLD);

    if (myRank != MASTER)
    {
        image1D = (unsigned char*)malloc(sizeof(unsigned char) * imgRows * imgColumns);
    }
    MPI_Bcast(image1D, imgRows*imgColumns, MPI_UNSIGNED_CHAR, MASTER, MPI_COMM_WORLD);

    int offset = imgRows/numOfProcs;
    int rowStart = myRank*offset;
    int rowEnd = rowStart + offset;

    //Add extra rows to the master portion
    if (myRank == MASTER)
    {
        rowEnd = rowEnd + imgRows%numOfProcs;
    }

    //For MPI_Gatherv.
    if (myRank == MASTER)
    {
        displs = (int*)malloc(sizeof(int) * numOfProcs);
        recivedCount = (int*)malloc(sizeof(int) * numOfProcs);

        for (int i = 0; i < numOfProcs; i++)
        {
            if (i == MASTER)
            {
                recivedCount[i] = (rowEnd-rowStart) * imgColumns;
            }
            else
            {
                recivedCount[i] = (rowEnd-(imgRows%numOfProcs)-rowStart) * imgColumns;
            }

            displs[i] = (i == 0) ? 0 : displs[i-1]+recivedCount[i-1];
        }
    }

    partialConvolvedImage = (unsigned char*)malloc(sizeof(unsigned char) * (rowEnd-rowStart)*imgColumns);

    threadpool thpool = thpool_init(1);

    for (int i = rowStart; i < rowEnd; i++)
    {

        for (int j = 0; j < imgColumns; j++)
        {
            Task* t = malloc(sizeof(Task));

            t->taskFunction = &sobel;
            t->image = image1D;
            t->partialConvolvedImage = partialConvolvedImage;
            t->imgRows = imgRows;
            t->imgColumns = imgColumns;
            t->i = i;
            t->j = j;
            t->rowStart = rowStart;

            thpool_add_work(thpool, (void*)sobel, (void*)t);
        
        }
    }

    thpool_wait(thpool);
    thpool_destroy(thpool);

    //Maybe use typdef to shorten the syntax
    MPI_Gatherv(partialConvolvedImage, (rowEnd-rowStart)*imgColumns, MPI_UNSIGNED_CHAR, convolvedImage, recivedCount, displs, MPI_UNSIGNED_CHAR, MASTER, MPI_COMM_WORLD);

    if (myRank == MASTER)
    {
        stbi_write_png("assets/OUT.png", imgColumns, imgRows, desiredChannels, convolvedImage, imgColumns * desiredChannels);        
    }

    stbi_image_free(image1D);
    free(partialConvolvedImage);

    if (myRank == MASTER)
    {
        free(convolvedImage);
    }
    MPI_Finalize();
    return 0;
}
