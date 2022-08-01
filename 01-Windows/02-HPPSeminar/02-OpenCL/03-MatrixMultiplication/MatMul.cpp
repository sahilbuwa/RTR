// Header Files
// Standard Header Files
#include<stdio.h>
#include<math.h> // fabs() sathi

// CUDA Headers
// OpenCL Headers
#include<CL/opencl.h>
#include"helper_timer.h"

// Macros
#define BLOCK_WIDTH 64

// Global Variables
cl_platform_id oclPlatformID;
cl_device_id oclDeviceID;

cl_context oclContext;
cl_command_queue oclCommandQueue;

cl_program oclProgram;
cl_kernel oclKernel;

int *hostA = NULL;
int *hostB = NULL;
int *hostC = NULL;
int *gold = NULL;

cl_mem deviceA = NULL;
cl_mem deviceB = NULL;
cl_mem deviceC = NULL;

float timeOnCPU = 0.0f;
float timeOnGPU = 0.0f;

// OpenCL Kernel
const char *oclSourceCode = 
"__kernel void matMulGPU(__global int *A, __global int *B, __global int *C, int numARows, int numAColumns, int numBColumns, int numCColumns)" \
"{" \
"int row = get_global_id(0);" \
"int column = get_global_id(1);" \
"if((row < numARows) && (column < numBColumns))" \
"{" \
"int value = 0;" \
"for(int k=0; k < numAColumns; k++)" \
"{" \
"int a = A[row * numAColumns + k];" \
"int b = B[k * numBColumns + column];" \
"value += a*b;" \
"}" \
"C[row * numCColumns + column] = value;" \
"}" \
"}";

int main(int argc, char *argv[])
{
    
}