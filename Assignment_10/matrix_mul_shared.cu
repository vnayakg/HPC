#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include "files.h"

#define SOFTENING 1e-9f

__global__ void initMatrix(int *a, int val, int N){

    int col = blockIdx.x * blockDim.x + threadIdx.x;  //col ind
    int row = blockIdx.y * blockDim.y + threadIdx.y; //row ind

    if(row < N and col < N){

        a[row*N + col] = val;
    }
}

void check(int *a, int *b, int *c, int N){
    int sum;

    for(int i=0; i<N; i++){
        for(int j = 0; j<N; j++){
            sum = 0;
            for(int k = 0; k<N; k++){
                sum += a[i* N + k] * b[k*N + i];
            }

            if(sum != c[i*N + j])
                printf("wrong answer\n");
        }
    }

    printf("check successful\n");
}

__global__ void matrixMul(int *a, int *b, int *c, int N){
    
    __shared__ int col;
    __shared__ int row;
    col = blockIdx.x * blockDim.x + threadIdx.x;  //col ind
    row = blockIdx.y * blockDim.y + threadIdx.y; //row ind

    if(row < N and col < N){

        int sum = 0;
        for(int i=0; i<N; i++){
            sum += a[row * N + i] * b[i*N + col];
        }

        c[row*N + col] = sum;
    }

}

int main(){
    // Set our square matrix dimension (2%10 x 2*10 default)
    
    int N=1 << 10;
    size_t bytes = N * N * sizeof(int);

    // Allocate memory for our matrices
    int *a, *b, *c;
    
    cudaMallocManaged(&a, bytes);
    cudaMallocManaged(&b, bytes);
    cudaMallocManaged(&c, bytes);
    
     int threads = 16;
    int blocks = (N + threads - 1) / threads;

    // kernel launch parameters
    dim3 THREADS(threads, threads);
    dim3 BLOCKS(blocks, blocks);

    initMatrix<<<BLOCKS, THREADS>>>(a, 1, N);
    initMatrix<<<BLOCKS, THREADS>>>(b, 1, N);

       
   

    // Launch 
    matrixMul<<<BLOCKS, THREADS>>>(a, b, c, N);
    cudaDeviceSynchronize();

    check(a, b, c, N);

}
