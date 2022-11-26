#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <stdlib.h>

#define N 100000000
#define MASTER 0
#define MAXPROCS 16

int dot_product();
void init_lst();
void print_lst();

int main()
{
    int *vector_x, *vector_y;
    int i, n;
    int prod, sidx, eidx, size;
    int pid, nprocs, rank;
    double stime, etime;
    MPI_Status status;
    MPI_Comm world;

    // n = 12;
    // if (n > N)
    // {
    //     printf("n=%d > N=%d\n", n, N);
    //     exit(1);
    // }

    MPI_Init(NULL, NULL);
    world = MPI_COMM_WORLD;
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);

    int portion = N / nprocs;
    sidx = pid * portion;
    eidx = sidx + portion;

    vector_x = (int *)malloc(sizeof(int) * N);
    vector_y = (int *)malloc(sizeof(int) * N);

    init_lst(vector_x, N);
    init_lst(vector_y, N);

    int tmp_prod[nprocs];
    for (i = 0; i < nprocs; i++)
        tmp_prod[i] = 0;

    stime = MPI_Wtime();

    if (pid == MASTER)
    {

        // printf("%d, %d, %d\n", sidx, eidx, n);
        prod = dot_product(sidx, eidx, vector_x, vector_y, N);
        for (i = 1; i < nprocs; i++)
            MPI_Recv(&tmp_prod[i - 1], 1, MPI_INT, i, 123, MPI_COMM_WORLD, &status);
    }
    else
    {
        // printf("%d, %d, %d\n", sidx, eidx, n);
        prod = dot_product(sidx, eidx, vector_x, vector_y, N);
        MPI_Send(&prod, 1, MPI_INT, MASTER, 123, MPI_COMM_WORLD);
    }

    if (pid == MASTER)
    {
        for (i = 0; i < nprocs; i++)
            prod += tmp_prod[i];
    }

    if (pid == MASTER)
    {
        // print_lst(vector_x,n);
        // print_lst(vector_y,n);
        etime = MPI_Wtime();
        printf("pid=%d: Dot Product = %d\n", pid, prod);
        printf("pid=%d: elapsed = %f\n", pid, etime - stime);
    }
    MPI_Finalize();

    free(vector_x);
    free(vector_y);
}

int dot_product(int s, int e, int x[], int y[], int n)
{
    int i, prod = 0;

    for (i = s; i < e; i++)
        prod = prod + x[i] * y[i];

    return prod;
}

void init_lst(int *l, int n)
{
    int i;
    for (i = 0; i < n; i++)
        *l++ = 1;
}
void print_lst(int *l, int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        printf("%d ", l[i]);
    }
    printf("\n");
}