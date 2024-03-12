#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <math.h>

#define PI 3.14159265358979323846

// CUDA kernel to compute the sine of angles in radians
__global__ void computeSine(const float *anglesRadians, float *sineValues, int n) {
    int index = threadIdx.x + blockIdx.x * blockDim.x;
    if (index < n) {
        sineValues[index] = sinf(anglesRadians[index]);
    }
}

int main() {
    int n = 1000;
    float *anglesRadians, *sineValues;
    float *d_anglesRadians, *d_sineValues;

    // Allocate host memory
    anglesRadians = (float *)malloc(n * sizeof(float));
    sineValues = (float *)malloc(n * sizeof(float));

    // Initialize angles with random values in radians
    srand(time(NULL)); // Seed the random number generator
    for (int i = 0; i < n; i++) {
        anglesRadians[i] = ((float)rand() / RAND_MAX) * 2 * PI; // Random angle in radians
    }

    // Allocate device memory
    cudaMalloc(&d_anglesRadians, n * sizeof(float));
    cudaMalloc(&d_sineValues, n * sizeof(float));

    // Copy angles to device
    cudaMemcpy(d_anglesRadians, anglesRadians, n * sizeof(float), cudaMemcpyHostToDevice);

    // Launch the kernel
    int blockSize = 256;
    int numBlocks = (n + blockSize - 1) / blockSize;
    computeSine<<<numBlocks, blockSize>>>(d_anglesRadians, d_sineValues, n);

    // Copy results back to host
    cudaMemcpy(sineValues, d_sineValues, n * sizeof(float), cudaMemcpyDeviceToHost);

    // Print angles in radians and their sine values
    printf("Angle (Radians)      sine(Angle)\n");
    for (int i = 0; i < n; i++) {
        printf("%f      %f\n", anglesRadians[i], sineValues[i]);
    }

    // Cleanup
    cudaFree(d_anglesRadians);
    cudaFree(d_sineValues);
    free(anglesRadians);
    free(sineValues);

    return 0;
}
