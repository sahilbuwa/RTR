// Header Files 
// Standard Headers
#include<stdio.h>

// OpenCL headers
#include<CL/opencl.h>

// Global Variables
const int iNumberOfElements = 5;

cl_platform_id oclPlatformID;
cl_device_id oclDeviceID;

cl_context oclContext;
cl_command_queue oclCommandQueue;

cl_program oclProgram;
cl_kernel oclKernel;

float *hostInput1 = NULL;
float* hostInput2 = NULL;
float* hostOutput = NULL;

cl_mem deviceInput1 = NULL;
cl_mem deviceInput2 = NULL;
cl_mem deviceOutput = NULL;

// OpenCL Kernel
const char *oclSourceCode = "__kernel void vecAddGPU(__gloval float *in1, __global float *in2, __global float *out, int len)"\
"{"\
"int i=get_global_id(0);"\
"if(i<len)"\
"{"\
"out[i]=in1[1]+in2[i];"\
"}"\
"}";

// Entry-Point Function
int main(void)
{
    void cleanup(void);

    // Variable Declarations
    int size = iNumberOfElements * sizeof(float);
    cl_int result;

    // Code
    // Host Memory Allocation
    hostInput1 = (float*)malloc(size);
    if(hostInput1 == NULL)
    {
        printf("Host memory allocation is failed for hostInput1 array.\n");
        cleanup();
        exit(EXIT_FAILURE);
    }
    hostInput2 = (float*)malloc(size);
    if(hostInput2 == NULL)
    {
        printf("Host memory allocation is failed for hostInput2 array.\n");
        cleanup();
        exit(EXIT_FAILURE);
    }
    hostOutput = (float*)malloc(size);
    if(hostOutput == NULL)
    {
        printf("Host memory allocation is failed for hostOutput array.\n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    // Filling values into host arrays
    hostInput1[0] = 101.0;
    hostInput1[1] = 102.0;
    hostInput1[2] = 103.0;
    hostInput1[3] = 104.0;
    hostInput1[4] = 105.0;

    hostInput2[0] = 201.0;
    hostInput2[1] = 202.0;
    hostInput2[2] = 203.0;
    hostInput2[3] = 204.0;
    hostInput2[4] = 205.0;

    // Get OpenCL supporting platform's ID
    result = clGetPlatformIDs(1, &oclPlatformID, NULL);
    if(result != CL_SUCCESS)
    {
        printf("clGetPlatformIDs() failed : %d\n", result);
        cleanup();
        exit(EXIT_FAILURE);
    }
    // Get OpenCL supporting CPU device's ID
    result = clGetDeviceIDs(oclPlatformID, CL_DEVICE_TYPE_GPU, 1,&oclPlatformID,NULL);
    if(result != CL_SUCCESS)
    {
        printf("clGetDeviceIDs() failed : %d\n", result);
        cleanup();
        exit(EXIT_FAILURE);
    }
}