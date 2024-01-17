#include <mpi.h>
#include <stdio.h>
#include <string.h>
int main (int argc, char* argv[])
{

    int var;
    int rank, size;
    char str[20];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;   

    if(rank == 0)
    {
        int x;
        printf("\nEnter a Number ");
        scanf("%d",&x);

        for(int i = 1;i<size;i++)
        {
            MPI_Send(&x, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else{
        MPI_Recv(&var, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        printf("\nRank :%d has recieved %d ",rank,var);
    }

    MPI_Finalize();
    return 0;

}