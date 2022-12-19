#include <mpi.h>
#include <thread>

#include "sobel.h"
#include "threadArgs.h"

#include "stb_image_write.h"
#include "image.h"


int main(int argc, char *argv[]) {

    int myRank, numOfProcs;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numOfProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    Image::Ptr image1D;
    Image::Ptr convolvedImage;

    int MASTER = numOfProcs - 1;

    int imgColumns, imgRows;
    int desiredChannels = 1;

    //For MPI_Gatherv.
    std::vector<int> displs(numOfProcs), recivedCount(numOfProcs);

    //Read image by master proc.
    if (myRank == MASTER) {

        image1D = Image::from_file("assets/9.jpg");
        imgColumns = image1D->columns();
        imgRows = image1D->rows();

        printf("Image Info: imgColumns:%d, imgRows:%d\n", imgColumns, imgRows);

        //Allocate array for output image.
        convolvedImage = Image::empty(imgRows, imgColumns);
    }

    MPI_Bcast(&imgRows, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
    MPI_Bcast(&imgColumns, 1, MPI_INT, MASTER, MPI_COMM_WORLD);

    if (myRank != MASTER) {
        image1D = Image::empty(imgRows, imgColumns);
    }
    MPI_Bcast(image1D->mut_data(), imgRows * imgColumns, MPI_UNSIGNED_CHAR, MASTER, MPI_COMM_WORLD);

    int offset = imgRows / numOfProcs;
    int rowStart = myRank * offset;
    int rowEnd = rowStart + offset;

    //Add extra rows to the master portion
    if (myRank == MASTER) {

        rowEnd = rowEnd + imgRows % numOfProcs;
    }

    //For MPI_Gatherv.
    if (myRank == MASTER) {

        for (int i = 0; i < numOfProcs; i++) {
            if (i == MASTER) {
                recivedCount[i] = (rowEnd - rowStart) * imgColumns;
            } else {
                recivedCount[i] = (rowEnd - (imgRows % numOfProcs) - rowStart) * imgColumns;
            }

            displs[i] = (i == 0) ? 0 : displs[i - 1] + recivedCount[i - 1];
        }
    }

    auto args = ThreadArgs{
            .image = image1D,
            .imgRows = imgRows,
            .rowStart = rowStart,
            .rowEnd = rowEnd,
            .imgColumns = imgColumns
    };

    auto convolvedImagePartPromise = std::promise<Image::Ptr>{};
    auto convolvedImagePartFuture = convolvedImagePartPromise.get_future();

    std::jthread thread{sobel, args, std::move(convolvedImagePartPromise)};
    auto convolvedImagePart = convolvedImagePartFuture.get();

    //Maybe use typdef to shorten the syntax
    MPI_Gatherv(
            convolvedImagePart->mut_data(),
            (rowEnd - rowStart) * imgColumns,
            MPI_UNSIGNED_CHAR,
            convolvedImage->mut_data(),
            recivedCount.data(),
            displs.data(),
            MPI_UNSIGNED_CHAR,
            MASTER,
            MPI_COMM_WORLD
    );

    if (myRank == MASTER) {
        stbi_write_png(
                "assets/OUT.png",
                imgColumns,
                imgRows,
                desiredChannels,
                convolvedImage->span().data(),
                imgColumns * desiredChannels
        );
    }

    MPI_Finalize();
    return 0;
}

