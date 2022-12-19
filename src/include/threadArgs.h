#pragma  once
typedef struct ThreadArgs {

    unsigned char* image;
    unsigned char** partialConvolvedImage;
    int imgRows;
    int rowStart;
    int rowEnd;
    int imgColumns;
} ThreadArgs;