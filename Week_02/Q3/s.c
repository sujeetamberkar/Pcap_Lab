#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char* argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Now arrayLength is equal to the number of processes
    int arrayLength = size;
    MPI_Status status;

    // Correct buffer size calculation
    int buffer_size = MPI_BSEND_OVERHEAD + (sizeof(int) * arrayLength);
    void* buffer = malloc(buffer_size);

    // Attach buffer
    MPI_Buffer_attach(buffer, buffer_size);

    if (rank == 0) {
        printf("Enter an Array of Size %d:\n", arrayLength);
        int array[arrayLength];
        for (int i = 0; i < arrayLength; i++) {
            scanf("%d", &array[i]);
        }

        for (int i = 1; i < size; i++) {
            MPI_Bsend(&array[i - 1], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        int receivedNumber;
        MPI_Recv(&receivedNumber, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        if (rank % 2 == 0) {
            receivedNumber *= receivedNumber; // Square for even ranks
        } else {
            receivedNumber = receivedNumber * receivedNumber * receivedNumber; // Cube for odd ranks
        }

        printf("Rank %d computed %d\n", rank, receivedNumber);
    }

    // Detach buffer
    MPI_Buffer_detach(&buffer, &buffer_size);
    free(buffer);
    MPI_Finalize();
    return 0;
}
