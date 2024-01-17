#include"mpi.h"
#include<stdio.h>

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(rank%2)
            printf("World");
    else
        printf("Hello");
    printf("\n");
    MPI_Finalize();




}