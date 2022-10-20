#include <stdio.h>
#include<omp.h>
#include <time.h>
#include <stdlib.h>

#define N 10000000

void scalerVectorAdd(int* a, int * c, int b){

    #pragma omp parallel for
    for(int i=0; i<N; i++){
        c[i] = a[i] + b;
    }

}

int main(){

    int* a = (int*)malloc(sizeof(int) * N);
    int b = 10;

    for(int i=0; i<N; i++){
        a[i] = 0;
    }

    int* c = (int*)malloc(sizeof(int) * N);

    double itime, ftime, exec_time;
    omp_set_num_threads(4);
    itime = omp_get_wtime();

    scalerVectorAdd(a, c, b);    
    ftime = omp_get_wtime();
    exec_time = ftime - itime;
    printf("\n\nTime taken is %f\n", exec_time);
    // for(int i=0; i<N; i++)
    //     printf("%d ", c[i]);
    free(a);
    free(c);
}