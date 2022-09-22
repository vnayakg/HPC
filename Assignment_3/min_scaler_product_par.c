// C Program to find the minimum scalar product of two vectors (dot product)
#include <stdio.h>
#include <time.h>
#include <omp.h>

#define n 10000

void swap(int *num1, int *num2)
{
    int temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}

void parallelSort(int arr[])
{
    int first;
    int i = 0, j = 0;
    for (i = 0; i < n; i++)
    {
        first = i % 2;
    #pragma omp parallel for default(none) shared(arr, first)
        for (j = first; j < n - 1; j += 2)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

void parallelSortReverse(int arr[])
{
    int first;
    int i = 0, j = 0;
    for (i = 0; i < n; i++)
    {
        first = i % 2;
        #pragma omp parallel for default(none) shared(arr, first)
        for (j = first; j < n - 1; j += 2)
        {
            if (arr[j] < arr[j + 1])
            {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

void sort(int arr[])
{
    int i, j;

    for (i = 0; i < n - 1; i++)
#pragma omp parallel for
        for (j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
}
void sort_des(int arr[])
{
    int i, j;
#pragma omp parallel for
    for (i = 0; i < n; ++i)
    {
#pragma omp parallel for
        for (j = i + 1; j < n; ++j)
        {
            if (arr[i] < arr[j])
            {
                int a = arr[i];
                arr[i] = arr[j];
                arr[j] = a;
            }
        }
    }
}

int main()
{
    // fill the code;
    // int n;
    // scanf("%d", &n);
    int arr1[n], arr2[n];
    int i;
    for (i = 0; i < n; i++)

    {
        // scanf("%d", &arr1[i]);
        arr1[i] = n - i;
    }
    for (i = 0; i < n; i++)
    {
        // scanf("%d", &arr2[i]);
        arr2[i] = i;
    }

    // clock_t t;
    // t = clock();

    
    omp_set_num_threads(6);

    double start; 
    double end; 
    start = omp_get_wtime(); 

    parallelSort(arr1);
    parallelSortReverse(arr2);
    // sort(arr1);
    // sort_des(arr2);

    end = omp_get_wtime(); 
    // t = clock() - t;
    //double time_taken = ((double)t)/CLOCKS_PER_SEC;

   
    printf("Time taken (seq): %f\n", end - start);

    int sum = 0;
    for (i = 0; i < n; i++)
    {
        sum = sum + (arr1[i] * arr2[i]);
    }
    printf("%d\n", sum);
    return 0;
}