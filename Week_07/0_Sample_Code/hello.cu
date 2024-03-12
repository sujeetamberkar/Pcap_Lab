#include <cuda_runtime.h>
#include <iostream>

__global__ void helloFromGPU(void) {
    if (threadIdx.x == 10) {
        printf("Hello World from GPU!\n");
    }
}

int main(void) {
    // Launch a kernel on the GPU with one thread for each element.
    helloFromGPU<<<1, 256>>>();
    
    // Wait for GPU to finish before accessing on host
    cudaDeviceSynchronize();
    
    return 0;
}
