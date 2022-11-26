#include <stdio.h>

#define MASK_DIM 7

#define MASK_OFFSET (MASK_DIM / 2)

__constant__ int mask[7 * 7];

__global__ void convolution_2d(int *matrix, int *result, int N)
{
    //global thread positions
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    // Starting index for calculation
    int start_r = row - MASK_OFFSET;
    int start_c = col - MASK_OFFSET;

    
    int temp = 0;

    // Iterate over all the rows
    for (int i = 0; i < MASK_DIM; i++)
    {
        // Go over each column
        for (int j = 0; j < MASK_DIM; j++)
        {
            
            if ((start_r + i) >= 0 && (start_r + i) < N && (start_c + j) >= 0 && (start_c + j) < N)
            {
                // result
                temp += matrix[(start_r + i) * N + (start_c + j)] * mask[i * MASK_DIM + j];
            
            }
        }
    }

    // Write the result
    result[row * N + col] = temp;
}

void init_matrix(int *m, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            m[n * i + j] = rand() % 100;
        }
    }
}

void verify_result(int *m, int *mask, int *result, int N)
{

    int temp;

    int offset_r;
    int offset_c;

    // Go over each row
    for (int i = 0; i < N; i++)
    {
        // Go over each column
        for (int j = 0; j < N; j++)
        {
            // Reset the temp variable
            temp = 0;

            // Go over each mask row
            for (int k = 0; k < MASK_DIM; k++)
            {
                // Update offset value for row
                offset_r = i - MASK_OFFSET + k;

                // Go over each mask column
                for (int l = 0; l < MASK_DIM; l++)
                {
                    // Update offset value for column
                    offset_c = j - MASK_OFFSET + l;

                    // Range checks if we are outside of the matrix
                    if (offset_r >= 0 && offset_r < N)
                    {
                        if (offset_c >= 0 && offset_c < N)
                        {
                            // Accumulate partial results
                            temp += m[offset_r * N + offset_c] * mask[k * MASK_DIM + l];
                        }
                    }
                }
            }
            // Fail if the results don't match
            if (result[i * N + j] != temp)
            {
                printf("Check failed");
                return;
            }
        }
    }
}

int main()
{

    int N = 1 << 10; // 2^10

    size_t bytes_n = N * N * sizeof(int);
    size_t bytes_m = MASK_DIM * MASK_DIM * sizeof(int);

    int *matrix;
    int *result;
    int *h_mask;

    cudaMallocManaged(&matrix, bytes_n);
    cudaMallocManaged(&result, bytes_n);
    cudaMallocManaged(&h_mask, bytes_m);

    init_matrix(matrix, N);
    init_matrix(mask, MASK_DIM);

    cudaMemcpyToSymbol(mask, h_mask, bytes_m);

    // Calculate grid dimensions
    int THREADS = 32;
    int BLOCKS = (N + THREADS - 1) / THREADS;

    // Dimension launch arguments
    dim3 block_dim(THREADS, THREADS);
    dim3 grid_dim(BLOCKS, BLOCKS);

    convolution_2d<<<grid_dim, block_dim>>>(matrix, result, N);

    verify_result(matrix, h_mask, result, N);

    printf("COMPLETED SUCCESSFULLY!");

    cudaFree(matrix);
    cudaFree(result);
    cudaFree(h_mask);

    return 0;
}
