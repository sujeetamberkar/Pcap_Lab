__kernel void ones_complement(__global int *A, __global int *B) {
    int i = get_global_id(0);
    int original = A[i];
    int complement = ~original; // Calculate the one's complement by flipping bits
    B[i] = complement;
}
