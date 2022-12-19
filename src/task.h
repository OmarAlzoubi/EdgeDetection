typedef struct Task {
    void (*taskFunction)(void* param);
    unsigned char* image;
    unsigned char* partialConvolvedImage;
    int imgRows;
    int rowStart;
    int imgColumns;
    int i;
    int j;

} Task;