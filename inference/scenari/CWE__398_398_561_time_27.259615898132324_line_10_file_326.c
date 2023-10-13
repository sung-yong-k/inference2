uda
#include <stdio.h>

#define N 1024

__global__ void MatrixMultiplication(float *A, float *B, float *C)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;
