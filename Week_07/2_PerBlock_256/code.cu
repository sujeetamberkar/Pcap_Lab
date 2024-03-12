#include <stdio.h>
// CUDA runtime
#include <cuda_runtime.h>

// Kernel definition
__global__ void vectorAdd(const int *A, const int *B, int *C, int numElements) {
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    if (i < numElements) {
        C[i] = A[i] + B[i];
    }
}

int main(void) {
    int numElements = 100; // Example size of vectors
    size_t size = numElements * sizeof(int);
    int *h_A = (int *)malloc(size);
    int *h_B = (int *)malloc(size);
    int *h_C = (int *)malloc(size);

    // Verify that allocations succeeded
    if (h_A == NULL || h_B == NULL || h_C == NULL) {
        fprintf(stderr, "Failed to allocate host vectors!\n");
        exit(EXIT_FAILURE);
    }

    // Initialize the host input vectors
    for (int i = 0; i < numElements; ++i) {
        h_A[i] = i;
        h_B[i] = i * 2;
    }

    // Allocate the device input vectors
    int *d_A = NULL;
    cudaMalloc((void **)&d_A, size);
    int *d_B = NULL;
    cudaMalloc((void **)&d_B, size);
    int *d_C = NULL;
    cudaMalloc((void **)&d_C, size);

    // Copy the host input vectors to the device
    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    // Launch the Vector Add CUDA Kernel
    int threadsPerBlock = 256;
    int blocksPerGrid = (numElements + threadsPerBlock - 1) / threadsPerBlock;
    printf("CUDA kernel launch with %d blocks of %d threads\n", blocksPerGrid, threadsPerBlock);
    vectorAdd<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, numElements);
    cudaError_t error = cudaGetLastError();
    if (error != cudaSuccess) {
        fprintf(stderr, "Failed to launch vectorAdd kernel (error code %s)!\n", cudaGetErrorString(error));
        exit(EXIT_FAILURE);
    }

    // Copy the device result vector in device memory to the host result vector in host memory.
    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

    // Print the results
    for (int i = 0; i < numElements; ++i) {
        printf("%d + %d = %d\n", h_A[i], h_B[i], h_C[i]);
    }
    
    // Free device global memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    // Free host memory
    free(h_A);
    free(h_B);
    free(h_C);

    printf("Done\n");
    return 0;
}
