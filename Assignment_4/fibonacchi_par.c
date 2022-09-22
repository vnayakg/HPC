#include <omp.h>
#include <stdio.h>

// recursive memoize fibonacci function
int fib(int n, int A[])
{
    int x, y;

    if (n < 2)
        return n;

    if (A[n] != 0)
        return A[n];

#pragma omp task shared(x, A)
    x = fib(n - 1, A);

#pragma omp task shared(y, A)
    y = fib(n - 2, A);

#pragma omp taskwait
    return A[n] = x + y;
}

int main()
{
    int n = 9, f;
    double start, end, duration;

    int A[n + 1] = {0};

    omp_set_num_threads(4);

    start = omp_get_wtime();

#pragma omp parallel
#pragma omp single
    f = fib(n, A);

    end = omp_get_wtime();
    duration = end - start;

    printf("fib(%d) = %d\n\n", n, f);
    printf("Time taken: %lf %15.15lf", duration, duration);

    return 0;
}