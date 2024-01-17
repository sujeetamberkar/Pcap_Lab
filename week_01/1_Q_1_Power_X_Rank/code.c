#include"mpi.h"
#include<stdio.h>
#include <math.h>

int powerfun (int a,int b)
{
    int product = 1;
    for(int i = 0;i<b;i++)
        product=product*a;
    return product;
}
int main(int argc, char *argv[])
{
    int rank, size;
    int x = 2;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    int ans = pow(x,rank);
    printf("x = %d, rank = %d , pow(x,rank) = %d",x,rank,ans);
    printf("\n");
    MPI_Finalize();




}