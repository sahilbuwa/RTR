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
    // Function Declarations
    void InitA(int *data, int, int);
    void InitB(int *data, int, int);
    void matMulCPU(int*, int*, int, int, int, int);
    void cleanup(void);

    // Variable Declarations
    int numARows = BLOCK_WIDTH;
    int numAColumns = BLOCK_WIDTH;
    int numBRows = BLOCK_WIDTH;
    int numBColumns = BLOCK_WIDTH;

    int numCRows = numARows;
    int numCColumns = numBColumns;

    int numGoldRows = numARows;
    int numGoldColumns = numBColumns;

    int sizeA = numARows * numAColumns * sizeof(int);
    int sizeB = numBRows * numBColumns * sizeof(int);
    int sizeC = numCRows * numCColumns * sizeof(int);
    int sizeGold = numGoldRows * numGoldColumns * sizeof(int);

    cl_int result;

    // Code
    // Host Memory Allocation
    hostA = (int *)malloc(sizeA);
    if(hostA == NULL)
    {
        printf("Host Memory allocation is failed for hostA.\n");
        cleanup();
        exit(EXIT_FAILURE);
    }
    hostB = (int *)malloc(sizeB);
    if(hostB == NULL)
    {
        printf("Host Memory allocation is failed for hostB.\n");
        cleanup();
        exit(EXIT_FAILURE);
    }
    hostC = (int *)malloc(sizeC);
    if(hostC == NULL)
    {
        printf("Host Memory allocation is failed for hostC.\n");
        cleanup();
        exit(EXIT_FAILURE);
    }
    gold = (int *)malloc(sizeGold);
    if(gold == NULL)
    {
        printf("Host Memory allocation is failed for gold.\n");
        cleanup();
        exit(EXIT_FAILURE);
    }
    
}