__kernel void swap_adjacent(__global int* A) {
    int i = get_global_id(0);

    int c = A[i];
    A[i] = A[i+2];
    A[i+2] = c;
}
