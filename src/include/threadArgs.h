#pragma  once
typedef struct ThreadArgs {
    void (*taskFunction)(void* param);
    unsigned char* image;
    unsigned char** partialConvolvedImage;
    int imgRows;
    int rowStart;
    int rowEnd;
    int imgColumns;

} ThreadArgs;