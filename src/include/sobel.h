#pragma once

#include <cmath>
#include <future>
#include "threadArgs.h"

namespace h2o {

    Image::Ptr combine(
            const Image::Ptr &gx,
            const Image::Ptr &gy,
            int imgRows,
            int imgColumns
    ) {

        auto image = Image::empty(imgRows, imgColumns);
        for (int i = 0; i < imgRows * imgColumns; i++) {

            double finalValue =
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
            int rowStart,
            int rowEnd,
            std::span<const int> kernel
    ) {

        constexpr int kRowCenter = 1;
        constexpr int kColumnCenter = 1;
        constexpr int kSize = 3;

        const auto columns = image->columns();

        auto partialConvolvedImage = Image::empty((rowEnd - rowStart), columns);

        for (int i = rowStart; i < rowEnd; i++) {
            for (int j = 0; j < columns; j++) {
                float newPixel = 0;
                for (int ki = 0; ki < kSize; ki++) {
                    for (int kj = 0; kj < kSize; kj++) {
                        //Center current pixel over the kernel.
                        int iCentered = i - kRowCenter + ki;
                        int jCentered = j - kColumnCenter + kj;

                        //Pad with zero.
                        if ((iCentered < 0 || iCentered >= image->rows()) || (jCentered < 0 || jCentered >= columns)) {
                            continue;
                        }

                        newPixel += static_cast<int>(image->at(iCentered * columns + jCentered)) *
                                    static_cast<int>(kernel[ki * kSize + kj]);
                    }
                }

                partialConvolvedImage->at((i - rowStart) * columns + j) = clamp(newPixel);
            }
        }

        return partialConvolvedImage;
    }

    void sobel(const ThreadArgs &task, std::promise<Image::Ptr> &&image_promise) {

        int rowStart = task.rowStart;
        int rowEnd = task.rowEnd;

        constexpr std::array<int, 9> sobelX{-1, 0, 1, -2, 0, 2, -1, 0, 1};
        constexpr std::array<int, 9> sobelY{1, 2, 1, 0, 0, 0, -1, -2, -1};

        auto gx = convolve(task.image, rowStart, rowEnd, sobelX);
        auto gy = convolve(task.image, rowStart, rowEnd, sobelY);
        if (gx->rows() != gy->rows() || gy->columns() != gy->columns())
            throw std::runtime_error("Image parts do not have the same size");

        image_promise.set_value(combine(gx, gy, rowEnd - rowStart, gx->columns()));
    }


}

