#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>


#define BUFSIZE 100

void Error_Handler(int error_code) {
  if (error_code != MPI_SUCCESS) {
    char error_string[BUFSIZE];
    int length_of_error_string, error_class;
    MPI_Error_class(error_code, & error_class);
    MPI_Error_string(error_code, error_string, & length_of_error_string);
    printf("[ERROR %d] : %s\n", error_class, error_string);
  }
}

int factorial(int a)
{
    int product = 1;
    for(int i = 1;i<=a;i++)
        product=product*i;
    return product;
}
int main (int argc, char * argv[])
{
    int rank, size, fact = 1, factsum,i;
    int error_code;
    MPI_Init(&argc,&argv);
    error_code = MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    error_code = MPI_Comm_size(MPI_COMM_WORLD,&size);
    Error_Handler(error_code);
    MPI_Barrier(MPI_COMM_WORLD);

    fact = factorial(rank+1); 
    MPI_Barrier(MPI_COMM_WORLD);

    error_code = MPI_Scan(&fact,&factsum,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
    Error_Handler(error_code);


    printf("Rank is %d and fact is %d and factsum is %d \n",rank,fact,factsum);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    exit(0);

}