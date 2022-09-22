#include <stdio.h>
#include<omp.h>
#include <time.h>

#define N 100000

int main(){

    int a[N];
    int b = 10;

    for(int i=0; i<N; i++){
        a[i] = 0;
    }

    int c[N];

    double itime, ftime, exec_time;
    itime = omp_get_wtime();
    for(int i=0; i<N; i++){
        c[i] = a[i] + b;
    }

    ftime = omp_get_wtime();
    exec_time = ftime - itime;
    printf("\n\nTime taken is %f\n", exec_time);
    // for(int i=0; i<N; i++)
    //     printf("%d ", c[i]);

}