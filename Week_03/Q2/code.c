#include "mpi.h"
#include <stdio.h>

float average(int array[], int size) {
    float sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += array[i];
    }
    return sum / size;
}


int main(int argc, char* argv[]) {
	int rank,size;
	float avg = 0;
	float b[100];
	int i, n, m;
	
	int arr[100],c[100];
	
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0) {
		n = size;
		fprintf(stdout,"Enter 'm': ");
		scanf("%d",&m);

		printf("Enter %d x %d = %d elements: ", n, m, n*m);
		for (i = 0; i < n * m; ++i) {
            scanf("%d",&arr[i]);
		}
		
	}

        MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
	    MPI_Scatter(arr, m, MPI_INT, c, m, MPI_INT, 0, MPI_COMM_WORLD);
    avg =average(c,m);
	MPI_Gather(&avg,1,MPI_FLOAT,b,1,MPI_FLOAT,0,MPI_COMM_WORLD);

	if (rank == 0) {
		
		float tavg = 0;
		for (i = 0; i < n; i++)
			tavg += b[i];
		tavg /= n;
		printf("Total average = %.2f\n", tavg);

	}

	MPI_Finalize();
}