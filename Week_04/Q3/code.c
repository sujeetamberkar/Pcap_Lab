#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>

#define BUFSIZE 100

void Error_Handler(int error_code)
{
    if (error_code != MPI_SUCCESS)
    {
        char error_string[BUFSIZE];
        int length_of_error_string, error_class;
        MPI_Error_class(error_code,&error_class);
        MPI_Error_string(error_code,error_string,&length_of_error_string);
        printf("%d %s\n",error_class,error_string);
    }
}

int main(int argc, char *argv[])
{
    int rank, size, element,total_count, count = 0;
    int mat [3][3];
    int error_code;
    
    MPI_Init(&argc,&argv);

    error_code=MPI_Comm_size(MPI_COMM_WORLD,&size);
    Error_Handler(error_code);
    error_code = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	Error_Handler(error_code);

    if (rank == 0)
    {
        printf("Enter 3 x 3 Matrix\n");
        for(int i = 0;i<3;i++)
            for(int j = 0; j<3;j++)\
                scanf("%d",&mat[i][j]);
        
    fflush(stdout);
    printf("Enter Element to be searched:\t");
    scanf("%d",&element);
    }

    for (int i = 0; i < 3; i++)
    {
        MPI_Bcast(mat[i],3,MPI_INT,0,MPI_COMM_WORLD);
    }
    MPI_Bcast(&element,1,MPI_INT,0,MPI_COMM_WORLD);

    for(int j = 0; j<3;j++)
        if(mat[rank][j]==element)
            count++;
    
    MPI_Reduce(&count,&total_count,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
    if(rank==0)
        printf("The count of element '%d' in Matrix is '%d'\n",element,total_count);
    MPI_Finalize();
    
    return 0;
}