#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define N 5

void hello_world(){
    printf("Hello world\n");
}

void bye(int i){
    printf("Bye: %d\n", i);
}

int main(){

   int* a = malloc(sizeof(int) * N);
   for(int i=0; i<N; i++){
       a[i] = 1;
   }

   #pragma omp parallel
   {
       #pragma omp for
       for(int i=0; i<N; i++){
           bye(i);
       }
       hello_world();
   }
}