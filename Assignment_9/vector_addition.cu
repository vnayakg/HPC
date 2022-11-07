#include <stdio.h>

void initWith(float num, float *a, int N)
{
    for (int i = 0; i < N; ++i)
    {
        a[i] = num;
    }
}

__global__ void addVectorsInto(float *result, float *a, float *b, int N)
{
    int index = threadIdx.x + blockIdx.x * blockDim.x;
    int stride = blockDim.x * gridDim.x;

    for (int i = index; i < N; i += stride)
    {
        result[i] = a[i] + b[i];
    }
}

void checkElementsAre(float target, float *vector, int N)
{
    for (int i = 0; i < N; i++)
    {
        if (vector[i] != target)
        {
            printf("FAIL: vector[%d] - %0.0f does not equal %0.0f\n", i, vector[i], target);
            exit(1);
        }
    }
    printf("Success! All values calculated correctly.\n");
}

int main()
{
    const int N = 2 << 24;
    size_t size = N * sizeof(float);

    float *a;
    float *b;
    float *c;

    cudaMallocManaged(&a, size);
    cudaMallocManaged(&b, size);
    cudaMallocManaged(&c, size);

    initWith(1, a, N);
    initWith(1, b, N);
    initWith(0, c, N);

    size_t threadsPerBlock;
    size_t numberOfBlocks;

    threadsPerBlock = 128;
    numberOfBlocks = 32;

    addVectorsInto<<<numberOfBlocks, threadsPerBlock>>>(c, a, b, N);
    cudaDeviceSynchronize();

    checkElementsAre(2, c, N);

    cudaFree(a);
    cudaFree(b);
    cudaFree(c);
}
