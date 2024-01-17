#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int main (int argc, char* argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;
    
    if (rank==0)
    {
        int x = 0;
        MPI_Send(&x,1,MPI_INT,1,0,MPI_COMM_WORLD);
    }
    else if (rank<size-1)
    {
        int temp;
        MPI_Recv(&temp, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
        temp = temp+ 1;
        MPI_Send(&temp, 1,MPI_INT,rank+1,0,MPI_COMM_WORLD);
    }
    else{
        int temp;
        MPI_Recv(&temp, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
        printf("Rank %d has recieved %d", rank,temp);
    }
    
    MPI_Finalize();
    return 0;
}