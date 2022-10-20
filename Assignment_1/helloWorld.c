#include <omp.h>
#include <stdio.h>

void helloWorld(int threadCount){
    omp_set_num_threads(threadCount);

    #pragma omp parallel
        {
            int th_num = omp_get_thread_num();

            printf("Hello World from thread no: %d\n", th_num);
        }
}

int main()
{

    int threadCount;

    printf("Enter No of threads: ");
    scanf("%d", &threadCount);

    helloWorld(threadCount);    
}