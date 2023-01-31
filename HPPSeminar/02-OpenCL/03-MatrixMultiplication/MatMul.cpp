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
	void matMulCPU(int*, int*, int*, int, int, int, int);
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

	// Printing matrix dimensions and sizes
	printf("The dimenstions of matrix 'hostA' are : %d x %d \n", numARows, numAColumns);
	printf("The dimenstions of matrix 'hostB' are : %d x %d \n", numBRows, numBColumns);
	printf("The dimenstions of matrix 'hostC' are : %d x %d \n", numCRows, numCColumns);
	printf("The dimenstions of matrix 'gold' are : %d x %d \n", numGoldRows, numGoldColumns);

	printf("Size of Matrix hostA = %d\n", sizeA);
	printf("Size of Matrix hostB = %d\n", sizeB);
	printf("Size of Matrix hostC = %d\n", sizeC);
	printf("Size of Matrix gold = %d\n", sizeGold);

	// Fill source matrices
	InitA(hostA, numARows, numAColumns);
	InitB(hostB, numBRows, numBColumns);

	// Get OpenCL supporting platform's ID
	result = clGetPlatformIDs(1, &oclPlatformID, NULL);
	if(result != CL_SUCCESS)
	{
		printf("clGetPlatformIDs() failed : %d.\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}
	
	// Get OpenCL supporting CPU device's ID
	result = clGetDeviceIDs(oclPlatformID, CL_DEVICE_TYPE_GPU, 1, &oclDeviceID, NULL);
	if(result != CL_SUCCESS)
	{
		printf("clGetDeviceIDs() failed : %d.\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}
	
	// Create OpenCL compute context
	oclContext = clCreateContext(NULL, 1, &oclDeviceID, NULL, NULL, &result);
	if(result != CL_SUCCESS)
	{
		printf("clCreateContext() failed : %d.\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// Create command queue
	oclCommandQueue = clCreateCommandQueue(oclContext, oclDeviceID, 0, &result);
	if(result != CL_SUCCESS)
	{
		printf("clCreateCommandQueue() failed : %d.\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// Create OpenCL program from .cl
	oclProgram = clCreateProgramWithSource(oclContext, 1, (const char **)&oclSourceCode, NULL, &result);
	if(result != CL_SUCCESS)
	{
		printf("clCreateProgramWithSource() failed : %d.\n", result);
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
		printf("Program build log : %s\n", buffer);
		printf("clBuildProgram() failed : %d\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// Create OpenCL kernel by passing kernel function name that we used in .cl file
	oclKernel = clCreateKernel(oclProgram, "matMulGPU", &result);
	if(result != CL_SUCCESS)
	{
		printf("clCreateKernel() failed : %d.\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// Device memory allocation
	deviceA = clCreateBuffer(oclContext, CL_MEM_READ_ONLY, sizeA, NULL, &result);
	if(result != CL_SUCCESS)
	{
		printf("clCreateBuffer() failed for 1st input matrix : %d.\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	deviceB = clCreateBuffer(oclContext, CL_MEM_READ_ONLY, sizeB, NULL, &result);
	if(result != CL_SUCCESS)
	{
		printf("clCreateBuffer() failed for 2nd input matrix : %d.\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	deviceC = clCreateBuffer(oclContext, CL_MEM_READ_ONLY, sizeC, NULL, &result);
	if(result != CL_SUCCESS)
	{
		printf("clCreateBuffer() failed for output matrix : %d.\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// Set 0 based 0th argument i.e. deviceA
	result = clSetKernelArg(oclKernel , 0, sizeof(cl_mem), (void *)&deviceA);
	if(result != CL_SUCCESS)
	{
		printf("clSetKernelArg() failed for 1st argument : %d.\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// Set 0 based 1st argument i.e. deviceB
	result = clSetKernelArg(oclKernel, 1, sizeof(cl_mem), (void *)&deviceB);
	if(result != CL_SUCCESS)
	{
		printf("clSetKernelArg() failed for 2nd argument : %d.\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// Set 0 based 2nd argument i.e. deviceC
	result = clSetKernelArg(oclKernel, 2, sizeof(cl_mem), (void *)&deviceC);
	if(result != CL_SUCCESS)
	{
		printf("clSetKernelArg() failed for 3rd argument : %d.\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// Set 0 based 3rd argument i.e. numARows
	result = clSetKernelArg(oclKernel, 3, sizeof(cl_int), (void *)&numARows);
	if(result != CL_SUCCESS)
	{
		printf("clSetKernelArg() failed for 4th argument : %d.\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// Set 0 based 4th argument i.e. numAColumns
	result = clSetKernelArg(oclKernel, 4, sizeof(cl_int), (void *)&numAColumns);
	if(result != CL_SUCCESS)
	{
		printf("clSetKernelArg() failed for 5th argument : %d.\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	} 

	// Set 0 based 5th argument i.e. numBColumns
	result = clSetKernelArg(oclKernel, 5, sizeof(cl_int), (void *)&numBColumns);
	if(result != CL_SUCCESS)
	{
		printf("clSetKernelArg() failed for 6th argument : %d.\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	} 

	// Set 0 based 6th argument i.e. numCColumns
	result = clSetKernelArg(oclKernel, 6, sizeof(cl_int), (void *)&numCColumns);
	if(result != CL_SUCCESS)
	{
		printf("clSetKernelArg() failed for 7th argument : %d.\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	} 

	// Write above 'input' device buffer to device memory
	result = clEnqueueWriteBuffer(oclCommandQueue, deviceA, CL_FALSE, 0, sizeA, hostA, 0, NULL, NULL);
	if(result != CL_SUCCESS)
	{
		printf("clEnqueueWriteBuffer() failed 1st input device buffer : %d.\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}
	
	result = clEnqueueWriteBuffer(oclCommandQueue, deviceB, CL_FALSE, 0, sizeB, hostB, 0, NULL, NULL);
	if(result != CL_SUCCESS)
	{
		printf("clEnqueueWriteBuffer() failed 2nd input device buffer : %d.\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// Kernel Configuration
	size_t globalWorkSize[2];
	globalWorkSize[0] = BLOCK_WIDTH;
	globalWorkSize[1] = BLOCK_WIDTH;

	// Start timer
	StopWatchInterface *timer = NULL;
	sdkCreateTimer(&timer);
	sdkStartTimer(&timer);

	result = clEnqueueNDRangeKernel(oclCommandQueue, oclKernel, 2, NULL, globalWorkSize, NULL, 0, NULL, NULL);
	if(result != CL_SUCCESS)
	{
		printf("clEnqueueNDRangeKernel() failed : %d.\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}
	// Finish Opencl command queue
	clFinish(oclCommandQueue);

	// Stop Timer
	sdkStopTimer(&timer);
	timeOnGPU = sdkGetTimerValue(&timer);
	sdkDeleteTimer(&timer);

	// Read back result from the  device (i.e from  deviceOutput) into cpu variable (i.e hostOutput)
	result = clEnqueueReadBuffer(oclCommandQueue, deviceC, CL_TRUE, 0, sizeC, hostC, 0, NULL, NULL);
	if(result != CL_SUCCESS)
	{
		printf("clEnqueueReadBuffer() failed : %d.\n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// Matrix multiplication on host
	matMulCPU(hostA, hostB, gold, numARows, numAColumns, numBColumns, numCColumns);

	// Comparison
	int breakValue = -1;
	bool bAccuracy = true;
	for(int i = 0; i < numCRows * numCColumns; i++)
	{
		int val1 = gold[i];
		int val2 = hostC[i];
		if(val1 != val2)
		{
			bAccuracy = false;
			breakValue = i;
			break;
		}
	}

	char str[128];
	if (bAccuracy == false)
		sprintf(str, "Comparison of CPU and GPU matrix multiplication is not accurate at array index %d", breakValue);
	else
		sprintf(str, "Comparison of CPU and GPU matrix multiplication is accurate.");

	printf("Timer taken for matrix multiplication on CPU = %.6f\n", timeOnCPU);
	printf("Timer taken for matrix multiplication on GPU = %.6f\n", timeOnGPU);
	printf("%s\n", str);

	// Cleanup
	cleanup();
	return 0;
}

void InitA(int *data, int row, int col)
{
	int num = 1;
	// Code
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			*(data + i * col + j) = num;
			num++;
		}
	}
}

void InitB(int *data, int row, int col)
{
	int num = BLOCK_WIDTH;
	// Code
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			*(data + i * col + j) = num;
			num--;
		}
	}
}

void matMulCPU(int *A, int* B, int* C, int numARows, int numAColumns, int numBColumns, int numCColumns)
{
	// Code
	StopWatchInterface *timer = NULL;
	sdkCreateTimer(&timer);
	sdkStartTimer(&timer);

	for (int i = 0; i < numARows; ++i)
	{
		for (int j = 0; j < numBColumns; ++j)
		{
			int value = 0.0f;
			for (int k = 0; k < numAColumns; ++k)
			{
				int a = A[i * numAColumns + k];
				int b = B[k * numBColumns + j];
				value += a * b;
			}
			C[i * numCColumns + j] = value;
		}

	}

	sdkStopTimer(&timer);
	timeOnCPU = sdkGetTimerValue(&timer);
	sdkDeleteTimer(&timer);
	timer = NULL;
}

void cleanup(void)
{
    // Code
    if(deviceC)
    {
        clReleaseMemObject(deviceC);
        deviceC= NULL;
    }
    if(deviceB)
    {
        clReleaseMemObject(deviceB);
        deviceB = NULL;
    }
    if(deviceA)
    {
        clReleaseMemObject(deviceA);
        deviceA = NULL;
    }
    if(oclKernel)
    {
        clReleaseKernel(oclKernel);
        oclKernel = NULL;
    }
    if(oclProgram)
    {
        clReleaseProgram(oclProgram);
        oclProgram = NULL;
    }
    if(oclCommandQueue)
    {
        clReleaseCommandQueue(oclCommandQueue);
        oclCommandQueue = NULL;
    }
    if(oclContext)
    {
        clReleaseContext(oclContext);
        oclContext = NULL;
    }
	if(gold)
    {
        free(gold);
        gold = NULL;
    }
    if(hostC)
    {
        free(hostC);
        hostC = NULL;
    }
    if(hostB)
    {
        free(hostB);
        hostB = NULL;
    }
    if(hostA)
    {
        free(hostA);
        hostA = NULL;
    }
}
