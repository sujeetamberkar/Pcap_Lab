#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h> // Include for printf

__global__ void add(int *a, int *b, int *c)
{
    *c = *a + *b;
}

int main(void) 
{
    int a, b, c; // host copies of variables a, b & c
    int *d_a, *d_b, *d_c; // device copies of variables a, b & c
    int size = sizeof(int);
    
    // Allocate space for device copies of a, b, c
    cudaError_t err;
    err = cudaMalloc((void **)&d_a, size);
    if (err != cudaSuccess) {
        printf("CUDA error: %s\n", cudaGetErrorString(err));
        return -1;
    }

    err = cudaMalloc((void **)&d_b, size);
    if (err != cudaSuccess) {
        printf("CUDA error: %s\n", cudaGetErrorString(err));
        return -1;
    }

    err = cudaMalloc((void **)&d_c, size);
    if (err != cudaSuccess) {
        printf("CUDA error: %s\n", cudaGetErrorString(err));
        return -1;
    }
    
    // Setup input values
    a = 3;
    b = 5;
    
    // Copy inputs to device
    cudaMemcpy(d_a, &a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, &b, size, cudaMemcpyHostToDevice);
    
    // Launch add() kernel on GPU
    add<<<1,1>>>(d_a, d_b, d_c);
    
    // Copy result back to host
    cudaMemcpy(&c, d_c, size, cudaMemcpyDeviceToHost);
    printf("Result: %d\n", c);
    
    // Cleanup
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
    
    return 0;
}
