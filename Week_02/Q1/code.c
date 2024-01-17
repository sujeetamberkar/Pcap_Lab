#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int rank, size;
    char str[20];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    if(rank == 0)
    {
        printf("Enter a word in Master Process ");
        fgets(str, 20, stdin);
        int length = strlen(str) + 1;

        // IF you want to send the word directly without mentioning the space give a random big value 
        MPI_Ssend(&length, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

        // Then send the string itself
        MPI_Ssend(str, length, MPI_CHAR, 1, 1, MPI_COMM_WORLD);
        fprintf(stdout, "I have sent %s from process 0 \n", str);

        MPI_Recv(str, length, MPI_CHAR, 1, 2, MPI_COMM_WORLD, &status);
        fprintf(stdout, "\nI have received %s in process 0\n", str);

    }
    else
    {
        int length;
        // First receive the length of the string
        MPI_Recv(&length, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        // Then receive the string itself
        MPI_Recv(str, length, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
        fprintf(stdout, "\nI have received %s in process 1\n", str);
        char var[20];
        for(int i = 0; i <length-1;i++)
        {
            if(str[i]>'Z')
            {
                str[i]=str[i] - 32;
            }
            else{
                str[i] = str[i] + 32;
            }
        }
        // printf("After Toggeling  1 has %s", str);
        MPI_Ssend(str, length, MPI_CHAR, 0, 2, MPI_COMM_WORLD);
        fprintf(stdout, "\nI have send %s from process 0\n", str);

    }

    MPI_Finalize();
    return 0;
}
