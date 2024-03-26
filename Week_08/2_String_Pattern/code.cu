#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declare device constant memory for the input string
__constant__ char d_str[64];

__global__ void Copy(char *rstr, int len) {
    int i = threadIdx.x;
    int startIdx = (len * (len + 1) / 2) - ((len - i) * (len - i + 1) / 2);

    for (int j = 0; j < len - i; j++) {
        rstr[startIdx + j] = d_str[j];
    }
}

int main() {
    char str[] = "PCAP";
    int len = strlen(str);
    int newsize =  (len*(len+1)) / 2; 
    char rstr[newsize]; // Ensure this is large enough to hold the result
    char *d_rstr;

    // Allocate device memory for the result string
    cudaMalloc((void**)&d_rstr, 300 * sizeof(char));
    cudaMemset(d_rstr, 0, 300 * sizeof(char)); // Initialize device memory to zeros

    // Copy the input string to constant memory on the device
    cudaMemcpyToSymbol(d_str, str, len * sizeof(char));

    // Launch the kernel
    Copy<<<1, len>>>(d_rstr, len);

    // Copy the result back to host memory
    cudaMemcpy(rstr, d_rstr,newsize* sizeof(char),cudaMemcpyDeviceToHost);
    rstr[newsize]='\0';
    printf("\nResultant String: %s\n", rstr);

    // Clean up
    cudaFree(d_rstr);
    return 0;
}
