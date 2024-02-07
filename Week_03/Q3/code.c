#include<mpi.h>
#include<stdio.h>
#include<string.h>

int main(int argc , char ** argv)
{
    int rank,size;
    int length;
    int storesize[100];
    char stringVar[20];
    char tempString[20];
    MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank==0)
    {
        fprintf(stdout,"Enter the length of string");
        scanf("%d",&length);
        while (getchar() != '\n');  // Clear the input buffer

        fprintf(stdout,"Enter a String\t of size (x * %d) ",size);
        fgets(stringVar,length,stdin);
    }

    MPI_Bcast(&length,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Scatter(stringVar,(length/size),MPI_CHAR,tempString,(length/size),MPI_CHAR,0, MPI_COMM_WORLD);

    int sum = 0;
    for(int i = 0;i <(length/size);i++)
    {
        if(tempString[i] == 'a' || tempString[i] == 'e' || tempString[i] == 'i' || tempString[i] == 'o' || tempString[i] == 'u')
        {
            sum++;
        }
        else {
            fprintf(stdout,"%c",tempString[i]);
        }
    }




    MPI_Gather(&sum,1,MPI_INT,storesize,1,MPI_INT,0,MPI_COMM_WORLD);

    if (rank==0)
    {
        int answer = 0;
        for(int i = 0;i<size;i++)
        {
            answer= answer+storesize[i];
        }
        fprintf(stdout,"Total Number of Non - vowels %d ",length-answer);
    }



MPI_Finalize();
}