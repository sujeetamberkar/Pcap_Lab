#include"mpi.h"
#include<stdio.h>
#include <string.h>
int main (int argc, char *argv[])
{
    int rank, size;
    char str[] ="Hello";


    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(rank>strlen(str))
    {
        printf("out of bound");
        MPI_Finalize();
        return 0;

    }
    if(str[rank] > 'Z')
        str[rank]=str[rank] - 32;
    else 
        str[rank] = str[rank] + 32;

    printf("%s",str);
    printf("\n");

    MPI_Finalize();
    return 0;
}