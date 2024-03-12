#include <cuda_runtime.h>
#include <stdio.h>

__global__ void vectorAdd(int *a, int *b, int *c, int N) {
    int index = threadIdx.x; // Since we're using N threads in a single block
    if (index < N) {
        c[index] = a[index] + b[index];
    }
}

int main() {
    int N = 100; // Now N is 100
    int *a, *b, *c;
    int *d_a, *d_b, *d_c;
    int size = N * sizeof(int);

    // Allocate memory on the host
    a = (int *)malloc(size);
    b = (int *)malloc(size);
    c = (int *)malloc(size);

    // Allocate memory on the device
    cudaMalloc(&d_a, size);
    cudaMalloc(&d_b, size);
    cudaMalloc(&d_c, size);

    // Initialize input vectors
    for (int i = 0; i < N; i++) {
        a[i] = i;
        b[i] = i * 2;
    }

    // Copy inputs to device
    cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);

    // Launch the vectorAdd kernel on 1 block with N threads
    vectorAdd<<<1, N>>>(d_a, d_b, d_c, N);

    // Copy result back to host
    cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);
    // Display the result
    for (int i = 0; i < N; i++) {
        printf("%d  +%d = %d\n",a[i],b[i],c[i]);
    }
    // Cleanup
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
    free(a);
    free(b);
    free(c);


    return 0;
}
