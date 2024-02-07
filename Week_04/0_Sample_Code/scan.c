#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char *argv[]) {
    int rank, size;
    int local_value = 5; // Each process has its local value
    int partial_sum; // Partial sum will be stored here

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Perform parallel prefix computation (scan) operation (sum) across all processes
    MPI_Scan(&local_value, &partial_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    // Print the partial sum on each process
    printf("Process %d partial sum: %d\n", rank, partial_sum);

    MPI_Finalize();
    return 0;
}
