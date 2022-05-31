// Header files
// Standard Headers
#include<stdio.h>
#include<math.h> // fabs()

// OpenCL headers
#include<CL/opencl.h>
#include"helper_timer.h"

// Global Variables
const int iNumberOfArrayElements = 11444777;

cl_platform_id oclPlatformID;
cl_device_id oclDeviceID;

cl_context oclContext;
cl_command_queue oclCommandQueue;

cl_program oclProgram;
cl_kernel oclKernel;

float *hostInput1 = NULL;
float *hostInput2 = NULL;
float *hostOutput = NULL;
float *gold = NULL;

cl_mem deviceInput1 = NULL;
cl_mem deviceInput2 = NULL;
cl_mem deviceOutput = NULL;

float timeOnCPU = 0.0f;
float timeOnGPU = 0.0f;

// OpenCL kernel
const char *oclSourceCode = 
"__kernel void vecAddGPU(__global float *in1, __global float *in2, __global float *out , int len)"\
"{"\
"int i=get_global_id(0);"\
"if(i<len)"\
"{"\
"out[i]=in1[i]+in2[i];"\
"}"\
"}";

// Entry point function
int main(void)
{
    // Function declarations
    void fillFloatArrayWithRandomNumbers(float*, int);
    size_t roundGlobalSizeToNearestMultipleOfLocalSize(int, unsigned int);
    void vecAddCPU(const float*, const float*, float*, int);
    void cleanup(void);
    // Variable Declarations
    int size = iNumberOfArrayElements * sizeof(float);
    cl_int result;

    // Code
    // Host memory allocation
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
    gold = (float*)malloc(size);
    if(gold == NULL)
    {
        printf("Host memory allocation is failed for gold array.\n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    // Filling values into host arrays
    fillFloatArrayWithRandomNumbers(hostInput1, iNumberOfArrayElements);
    fillFloatArrayWithRandomNumbers(hostInput2, iNumberOfArrayElements);

    // Get opencl supporting platform's ID
    result = clGetPlatformIDs(1, &oclPlatformID, NULL);
    if(result != CL_SUCCESS)
    {
        printf("clGetPlatformIDs() failed : %d\n", result);
        cleanup();
        exit(EXIT_FAILURE);
    }
    
    // Create OpenCL compute context 
    oclContext = clCreateContext(NULL, 1, &oclDeviceID, NULL, NULL, &result);
    if(result != CL_SUCCESS)
    {
        printf("clCreateContext() failed : %d\n", result);
        cleanup();
        exit(EXIT_FAILURE);
    }

    // Create Command Queue
    oclCommandQueue = clCreateCommandQueue(oclContext, oclDeviceID, 0, &result);
    if(result != CL_SUCCESS)
    {
        printf("clCreateCommandQueue() failed : %d\n", result);
        cleanup();
        exit(EXIT_FAILURE);
    }

    // Create Opencl program from .cl
    oclProgram = clCreateProgramWithSource(oclContext, 1, (const char **)&oclSourceCode, NULL, &result);
    if(result != CL_SUCCESS)
    {
        printf("clCreateProgramWithSource() failed : %d\n", result);
        cleanup();
        exit(EXIT_FAILURE);
    }

    // Build OpenCL program
    result = clBuildProgram(oclProgram, 0, NULL, NULL, NULL, NULL);
    if(result != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];
        clGetProgramBuildInfo(oclProgram, oclDeviceID, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("Program Build Log : %s\n", buffer);
        printf("clBuildProgram() failed : %d\n", result);
        cleanup();
        exit(EXIT_FAILURE);
    }
    
}