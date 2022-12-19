#pragma  once

#include "image.h"

struct ThreadArgs {

    Image::Ptr image;
    int imgRows;
    int rowStart;
    int rowEnd;
    int imgColumns;
};