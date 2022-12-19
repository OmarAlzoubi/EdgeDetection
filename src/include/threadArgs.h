#pragma  once

#include "image.h"

struct ThreadArgs {

    h2o::Image::Ptr image;
    int rowStart;
    int rowEnd;
};