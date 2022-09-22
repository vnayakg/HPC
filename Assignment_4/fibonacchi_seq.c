#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int fib(int n)
{
    /* Declare an array to store Fibonacci numbers. */
    int f[n + 2]; // 1 extra to handle case, n = 0
    int i;

    /* 0th and 1st number of the series are 0 and
    1*/
    f[0] = 0;
    f[1] = 1;

    for (i = 2; i <= n; i++)
    {
        /* Add the previous 2 numbers in the
        series and store it */

        f[i] = f[i - 1] + f[i - 2];
    }
    return f[n];
}

int main()
{
    int n = 20;

    double start, end;
    int fib_ans;
    start = omp_get_wtime();
    
    fib_ans = fib(n);

    end = omp_get_wtime();

    printf("Time Required (seq): %f\n", end - start);
    printf("Answer: %d\n", fib_ans);

    return 0;
}