#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int main (int argc, char* argv[])
{
    int arrayLength = 5;
    int rank, size;
    char str[20];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;
    
    // Correct buffer size calculation
    int buffer_size = MPI_BSEND_OVERHEAD + (sizeof(int) * arrayLength);
    void* buffer = malloc(buffer_size); 

    // Attach buffer
    MPI_Buffer_attach(buffer, buffer_size);

    if(rank==0)
    {
        printf("Enter a Array of Size %d \n",arrayLength-1);
        int array[arrayLength];
        for(int i = 0 ; i <arrayLength-1;i++)
        {
            scanf("%d",&array[i]);
        }

        for(int i = 1;i<arrayLength;i++)
        {
            int temp = array[i-1];
            MPI_Bsend(&temp,1,MPI_INT,i,0,MPI_COMM_WORLD);
        }

    }

else if(rank < arrayLength + 1)
{
    int receivedNumber;
    MPI_Recv(&receivedNumber, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

    if(rank%2==0)
    {
        printf("Rank %d received %d Square is %d\n", rank, receivedNumber,receivedNumber*receivedNumber);

    }
    else {
        printf("Rank %d received %d Cube is %d\n", rank, receivedNumber,receivedNumber*receivedNumber*receivedNumber);

    }
}

     // Detach buffer
    MPI_Buffer_detach(&buffer, &buffer_size);
    free(buffer);
    MPI_Finalize();
    return 0;
}