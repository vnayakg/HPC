#include <stdio.h>
#include <time.h>
#include <omp.h>

int main(){

    // 
    double itime, ftime, exec_time;
    int num;
    printf("Enter num: ");
    scanf("%d", &num);
    
    
    long long ans = 0;
    itime = omp_get_wtime();
    for(int i = 1; i <=num; i++){
        ans += i*i;
    }

    ftime = omp_get_wtime();
    exec_time = ftime - itime;

    printf("Sum of squares of numbers form 1 to 100 is: %lld\n", ans);
    printf("Time taken (seq): %f\n", exec_time);
}