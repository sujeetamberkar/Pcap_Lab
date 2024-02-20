__kernel void ones_complement(__global int *A, __global int *B) {
    int i = get_global_id(0);

    int binary = A[i]; // Get the binary number from input array
    int decimal = 0;
    int base = 1;
    int remainder;

    while (binary > 0) {
        remainder = binary % 10; // Extract the last digit of the binary number
        binary = binary / 10;    // Remove the last digit from the number
        decimal += remainder * base;
        base = base * 2;         // Increase the base (2^0, 2^1, 2^2, ...)
    }

    B[i] = decimal; // Store the computed decimal number in the output array
}
