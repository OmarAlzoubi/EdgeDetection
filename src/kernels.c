#include "kernels.h"

int emboss [3][3] = {{-2,-1,0},{-1,1,1},{0,1,2}};
int sobelX [3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
int sobelY [3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
int preWittX [3][3] = {{-1,0,1},{-1,0,1},{-1,0,1}};
int preWittY [3][3] = {{-1,-1,-1},{0,0,0},{1,2,1}};

int kRows = 3;
int kColumns = 3;