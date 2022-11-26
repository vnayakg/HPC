#include <stdio.h>

void initWith(float val, float *arr, int N)
{
  for (int i = 0; i < N; i++)
  {
    arr[i] = val;
  }
}

__global__ void prefixSum(float *arr, float *res, float *ptemp, float *ttemp, int N)
{
  int threadId = blockIdx.x * blockDim.x + threadIdx.x;
  int totalThreads = gridDim.x * blockDim.x;
  int elementsPerThread = ceil(1.0 * N / totalThreads);

  int start = threadId * elementsPerThread;
  int count = 0;
  float *sums = new float[elementsPerThread];
  float sum = 0;

  for (int i = start; i < N && count < elementsPerThread; i++, count++)
  {
    sum += arr[i];
    sums[count] = sum;
  }

  float localSum;
  if (count)
    localSum = sums[count - 1];
  else
    localSum = 0;
  ptemp[threadId] = localSum;
  ttemp[threadId] = localSum;

  __syncthreads();

  if (totalThreads == 1)
  {
    for (int i = 0; i < N; i++)
      res[i] = sums[i];
  }
  else
  {
    int d = 0; // log2(totalThreads)
    int x = totalThreads;
    while (x > 1)
    {
      d++;
      x = x >> 1;
    }

    x = 1;
    for (int i = 0; i < 2 * d; i++)
    {
      int tsum = ttemp[threadId];

      __syncthreads();

      int newId = threadId / x;
      if (newId % 2 == 0)
      {
        int nextId = threadId + x;
        ptemp[nextId] += tsum;
        ttemp[nextId] += tsum;
      }
      else
      {
        int nextId = threadId - x;
        ttemp[nextId] += tsum;
      }

      x = x << 1;
    }

    __syncthreads();

    float diff = ptemp[threadId] - localSum;
    for (int i = start, j = 0; i < N && j < count; i++, j++)
    {
      res[i] = sums[j] + diff;
    }
  }
}

void checkRes(float *arr, float *res, int N)
{
  float sum = 0;
  for (int i = 0; i < N; i++)
  {
    sum += arr[i];
    if (sum != res[i])
    {
      printf("FAIL: res[%d] - %0.0f does not equal %0.0f\n", i, res[i], sum);
      exit(1);
    }
  }
  printf("SUCCESS! All prefix sums added correctly.\n");
}

int main()
{
  const int N = 1000000;
  size_t size = N * sizeof(float);

  float *arr;
  float *res;

  cudaMallocManaged(&arr, size);
  cudaMallocManaged(&res, size);

  initWith(2, arr, N);
  initWith(0, res, N);

  int blocks = 1;
  int threadsPerBlock = 16;
  int totalThreads = blocks * threadsPerBlock;

  float *ptemp;
  float *ttemp;
  cudaMallocManaged(&ptemp, totalThreads * sizeof(float));
  cudaMallocManaged(&ttemp, totalThreads * sizeof(float));

  prefixSum<<<blocks, threadsPerBlock>>>(arr, res, ptemp, ttemp, N);
  cudaDeviceSynchronize();

  checkRes(arr, res, N);

  cudaFree(arr);
  cudaFree(res);
  cudaFree(ttemp);
  cudaFree(ptemp);
}