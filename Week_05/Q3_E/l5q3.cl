__kernel void swap(__global int *nums) {
  int x = get_global_id(0);
  int idx = x * 2;
  int temp = nums[idx];
  nums[idx] = nums[idx + 1];
  nums[idx + 1] = temp;
}
