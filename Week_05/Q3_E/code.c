#include <stdio.h>
#include <OpenCL/opencl.h>
#include <stdlib.h>
#define MAX_SOURCE_SIZE 100000
int main()
{
    int N;
    int *nums = (int *)malloc(100 * sizeof(int));
    FILE *fp;
    printf("Enter N: ");
    scanf("%d", &N);
    printf("Enter elements: ");
    for (int i = 0; i < N; i++)
    {
        scanf("%d", &nums[i]);
    }
    fp = fopen("l5q3.cl", "r");
    if (!fp)
    {
        printf("Error\n");
        exit(1);
    }

    // get device and platform id
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret;
    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);

    // create a context
    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

    // create a command queue
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);

    // Create buffers on the device
    cl_mem nums_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, N * sizeof(int), NULL, &ret);

    // copy data to buffers
    ret = clEnqueueWriteBuffer(command_queue, nums_obj, CL_TRUE, 0, N * sizeof(int), nums, 0, NULL, NULL);

    // create OpenCL program
    char *source_str;
    size_t source_size;
    source_str = (char *)malloc(MAX_SOURCE_SIZE);
    source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);
    cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

    // Build the program
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    // Create the kernel object
    cl_kernel kernel = clCreateKernel(program, "swap", &ret);

    // set arguments of kernel
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&nums_obj);

    size_t global_item_size = N / 2;
    size_t local_item_size = 1;

    // Execute kernel on device
    cl_event event;
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

    ret = clFinish(command_queue);

    // read memory buffer to local variable
    ret = clEnqueueReadBuffer(command_queue, nums_obj, CL_TRUE, 0, N * sizeof(int), nums, 0, NULL, NULL);

    // display results
    for (int i = 0; i < N; i++)
    {
        printf("%d ", nums[i]);
    }

    // clean up
    ret = clFlush(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(nums_obj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
    free(source_str);
    return 0;
}