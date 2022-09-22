#include <stdio.h>
#include <omp.h>
#include <time.h>

int main() {
   
    double itime, ftime, exec_time;

    int num;
    printf("Enter num: ");
    scanf("%d", &num);

    itime = omp_get_wtime();
    long long ans = 0;
    int i;

    #pragma omp parallel for num_threads(4) reduction(+:ans)
        for(i = 1; i <=num; i++) {
           
            ans += i*i;
        }
        
        
    ftime = omp_get_wtime();
    exec_time = ftime - itime;
    printf("Sum of squares of numbers form 1 to 100 is: %lld\n", ans);
    printf("\n\nTime taken is %f\n", exec_time);
}