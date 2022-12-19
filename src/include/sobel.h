#pragma once

#include <cmath>
#include <future>
#include "kernels.h"
#include "threadArgs.h"

using namespace std::literals;

Image::Ptr combine(const Image::Ptr &gx, const Image::Ptr &gy, int imgRows, int imgColumns) {

    auto image = Image::empty(imgRows, imgColumns);
    for (int i = 0; i < imgRows * imgColumns; i++) {

        auto finalValue =
                hypot(
                        static_cast<unsigned char>(gx->at(i)),
                        static_cast<unsigned char>(gy->at(i))
                );

        if (finalValue > 255) {
            finalValue = 255;
        }

        image->at(i) = static_cast<std::byte>(finalValue);
    }
    return image;
}

std::byte clamp(float pixel) {

    if (pixel < 0) {
        pixel = 0;
    } else if (pixel > 255) {
        pixel = 255;
    }

    return static_cast<std::byte>(pixel);
}

Image::Ptr convolve(
        const Image::Ptr &image,
        int imgRows,
        int imgColumns,
        int rowStart,
        int rowEnd,
        std::span<const int> kernel
) {

    constexpr int kRowCenter = 1;
    constexpr int kColumnCenter = 1;

    auto partialConvolvedImage = Image::empty((rowEnd - rowStart), imgColumns);

    for (int i = rowStart; i < rowEnd; i++) {
        for (int j = 0; j < imgColumns; j++) {
            float newPixel = 0;
            for (int ki = 0; ki < kRows; ki++) {
                for (int kj = 0; kj < kColumns; kj++) {
                    //Center current pixel over the kernel.
                    int iCentered = i - kRowCenter + ki;
                    int jCentered = j - kColumnCenter + kj;

                    //Pad with zero.
                    if ((iCentered < 0 || iCentered >= imgRows) || (jCentered < 0 || jCentered >= imgColumns)) {
                        continue;
                    }

                    newPixel += static_cast<int>(image->at(iCentered * imgColumns + jCentered)) *
                                static_cast<int>(kernel[ki * kColumns + kj]);
                }
            }

            partialConvolvedImage->at((i - rowStart) * imgColumns + j) = clamp(newPixel);
        }
    }

    return partialConvolvedImage;
}

void sobel(const ThreadArgs &task, std::promise<Image::Ptr> &&image_promise) {

    int imgRows = task.imgRows;
    int rowStart = task.rowStart;
    int rowEnd = task.rowEnd;
    int imgColumns = task.imgColumns;
    constexpr std::array<int, 9> sobelX{-1, 0, 1, -2, 0, 2, -1, 0, 1};
    constexpr std::array<int, 9> sobelY{1, 2, 1, 0, 0, 0, -1, -2, -1};

    auto gx = convolve(task.image, imgRows, imgColumns, rowStart, rowEnd, sobelX);
    auto gy = convolve(task.image, imgRows, imgColumns, rowStart, rowEnd, sobelY);

    image_promise.set_value(combine(gx, gy, rowEnd - rowStart, imgColumns));
}

