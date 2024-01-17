#include"mpi.h"
#include<stdio.h>
int main (int argc, char *argv[])
{
    int rank, size;

    int a = 15;
    int b = 3;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    int ans;
    switch(rank)
    {
        case 0:
        ans = a+b;
        printf("%d + %d=",a,b);
        break;

        case 1:
        ans = a-b;
        printf("%d - %d=",a,b);
        break;

        case 2:
        ans = a*b;
        printf("%d * %d=",a,b);

        break;

        case 3:
        ans = a/b;
        printf("%d / %d=",a,b);
        break;



        case 4:
        ans = a % b;
        printf("%d mod %d=",a,b);
        break;




        default:
        printf("Out of bound");
        return 0;
    }
    printf("%d\n",ans);
    MPI_Finalize();
    return 0;
}