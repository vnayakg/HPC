#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#define N 10000000

void vectorAdd(int* a, int* b, int* c){
    int i;
    #pragma omp parallel for num_threads(6)
        for(i=0; i<N; i++){
            c[i] = a[i] + b[i];
        }
    
}

int main(){

    int* a = (int *)malloc(sizeof(int)*N);
    int* b = (int *)malloc(sizeof(int)*N);


    for(int i=0; i<N; i++){
        a[i] = 1;
        b[i] = 1;
    }

    int* c = (int *)malloc(sizeof(int)*N);

    double itime, ftime, exec_time;
    itime = omp_get_wtime();
    vectorAdd(a, b, c);
    ftime = omp_get_wtime();
    exec_time = ftime - itime;
    printf("\n\nTime taken is %f\n", exec_time);

    // for(int i=0; i<N; i++)
    //     printf("%d ", c[i]);
    free(a);
    free(b);
    free(c);
}