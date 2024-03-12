#include <stdio.h>
#include <cuda_runtime.h>

__global__ void conv1D(int *N, int *M, int *P, int width, int mask_width) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int k;
    int Pvalue = 0;
    int N_start_point = i - (mask_width / 2);
    for (k = 0; k < mask_width; k++) {
        if (N_start_point + k >= 0 && N_start_point + k < width) {
            Pvalue += N[N_start_point + k] * M[k];
        }
    }
    if (i < width) {
        P[i] = Pvalue;
    }
}

int main() {
    int width = 100;
    int mask_width = 7;
    int *N, *M, *P;
    int *d_N, *d_M, *d_P;

    // Allocate host memory
    N = (int *)malloc(width * sizeof(int));
    M = (int *)malloc(mask_width * sizeof(int));
    P = (int *)malloc(width * sizeof(int));

    // Initialize host arrays
    for (int i = 0; i < width; i++) {
        N[i] = i; // Example: Initialize all elements to 1 for simplicity
    }
    for (int i = 0; i < mask_width; i++) {
        M[i] = i; // Example: Initialize all mask elements to 1 for simplicity
    }

    // Allocate device memory
    cudaMalloc(&d_N, width * sizeof(int));
    cudaMalloc(&d_M, mask_width * sizeof(int));
    cudaMalloc(&d_P, width * sizeof(int));

    // Copy from host to device
    cudaMemcpy(d_N, N, width * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_M, M, mask_width * sizeof(int), cudaMemcpyHostToDevice);

    // Define block size and number of blocks
    int threadsPerBlock = 256;
    int blocksPerGrid = (width + threadsPerBlock - 1) / threadsPerBlock;

    // Launch the kernel
    conv1D<<<blocksPerGrid, threadsPerBlock>>>(d_N, d_M, d_P, width, mask_width);

    // Copy result back to host
    cudaMemcpy(P, d_P, width * sizeof(int), cudaMemcpyDeviceToHost);


    // Print N
    printf("N = ");
    for (int i = 0; i < width; i++) {
        printf("%d ", N[i]);
    }
    printf("\n");

    // Print M
    printf("M = ");
    for (int i = 0; i < mask_width; i++) {
        printf("%d ", M[i]);
    }
    printf("\n");

    // Print P
    printf("P = ");
    for (int i = 0; i < width; i++) {
        printf("%d ", P[i]);
    }
    printf("\n");


    // Free device memory
    cudaFree(d_N);
    cudaFree(d_M);
    cudaFree(d_P);

    // Free host memory
    free(N);
    free(M);
    free(P);

    return 0;
}
