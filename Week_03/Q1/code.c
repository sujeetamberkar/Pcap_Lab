#include <mpi.h>
#include <stdio.h>


int factorial(int num)
{
    int product = 1;
    while (num)
    {
        product = product * num;
        num = num - 1;
    }
    return product;
    
}
int main(int argc, char *argv[])
{
    int rank, size, N, A[10], B[10], c, i;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        N = size;
        fprintf(stdout, "Enter %d values: \n", N);
        fflush(stdout);
        for (int i = 0; i < N; i++)
        {
            scanf("%d", &A[i]);
        }
    }
    fflush(stdout);


    MPI_Scatter(A, 1, MPI_INT, &c, 1, MPI_INT, 0, MPI_COMM_WORLD);
    fprintf(stdout, "I have recieved %d in process %d \n", c, rank);

    c = factorial(c);

    MPI_Gather(&c, 1, MPI_INT, B, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        fprintf(stdout, "The Result gathered in root is \n");
        fflush(stdout);
        int sum = 0;
        for (int i = 0; i < N; i++)
        {
            fprintf(stdout, "%d\t", B[i]);
            sum = sum + B[i];
        }
        
        fflush(stdout);
        fprintf(stdout,"\nThe total is %d",sum);
    }
    MPI_Finalize();
    return 0;
}