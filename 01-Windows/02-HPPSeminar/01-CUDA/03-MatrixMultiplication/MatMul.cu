#include<stdio.h>

// Cuda headers
#include<cuda.h>
#include"helper_timer.h"

// Macros
#define BLOCK_WIDTH 32

// Global Variables
int *hostA = NULL;
int *hostB = NULL;
int *hostC = NULL;
int *gold = NULL;

int *deviceA = NULL;
int *deviceB = NULL;
int *deviceC = NULL;

float timeOnCPU = 0.0f;
float timeOnGPU = 0.0f;

// Cuda Kernel Function
__global__ void matMulGPU(int *A, int *B,int *C, int numARows, int numAColumns, int numBColumns, int numCColumns)
{
    // Variable Declarations
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int column = blockIdx.x * blockDim.x + threadIdx.x;
    // Code
    if((row < numARows) && (column < numBColumns))
    {
        int value = 0.0f;
        for(int k=0; k < numAColumns; k++)
        {
            int a = A[row * numAColumns + k];
            int b = B[k * numBColumns + column];
            value += a*b;
        }
        C[row * numCColumns + column] = value;
    }
}

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

    cudaError_t result = cudaSuccess;

    // Code
    // Host Memory Allocation
    hostA = (int*)malloc(sizeA);
    if(hostA == NULL)
    {
        printf("Host Memory allocation is failed for hostA matrix.\n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    hostB = (int*)malloc(sizeB);
    if(hostB == NULL)
    {
        printf("Host Memory allocation is failed for hostB matrix.\n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    hostC = (int*)malloc(sizeC);
    if(hostC == NULL)
    {
        printf("Host Memory allocation is failed for hostC matrix.\n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    gold = (int*)malloc(sizeGold);
    if(gold == NULL)
    {
        printf("Host Memory allocation is failed for gold matrix.\n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    // Printing matrix dimensions and sizes
    printf("The dimensions of matrix hostA are : %d x %d \n",numARows, numAColumns);
    printf("The dimensions of matrix hostB are : %d x %d \n",numBRows, numBColumns);
    printf("The dimensions of matrix hostC are : %d x %d \n",numCRows, numCColumns);
    printf("The dimensions of matrix gold are : %d x %d \n",numGoldRows, numGoldColumns);

    printf("Size of Matrix hostA = %d\n", sizeA);
    printf("Size of Matrix hostB = %d\n", sizeB);
    printf("Size of Matrix hostC = %d\n", sizeC);

    printf("Size of Matrix gold = %d\n", sizeGold);

    // Fill Source Matrices
    InitA(hostA, numARows, numAColumns);
    InitB(hostB, numBRows, numBColumns);

    // Device memory allocation
    result = cudaMalloc((void**)&deviceA, sizeA);
    if(result != cudaSuccess)
    {
        printf("Device Memory allocation failed for deviceA matrix.\n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    result = cudaMalloc((void**)&deviceB, sizeB);
    if(result != cudaSuccess)
    {
        printf("Device Memory allocation failed for deviceB matrix.\n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    result = cudaMalloc((void**)&deviceC, sizeC);
    if(result != cudaSuccess)
    {
        printf("Device Memory allocation failed for deviceC matrix.\n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    // Copy data from host matrices into device matrices 
    result = cudaMemcpy(deviceA , hostA, sizeA, cudaMemcpyHostToDevice);
    if(result != cudaSuccess)
    {
        printf("Host to device data copy is failed for deiceA matrix.\n");
        cleanup();
        exit(EXIT_FAILURE);
    }
    result = cudaMemcpy(deviceB , hostB, sizeB, cudaMemcpyHostToDevice);
    if(result != cudaSuccess)
    {
        printf("Host to device data copy is failed for deiceA matrix.\n");
        cleanup();
        exit(EXIT_FAILURE);
    }
    
    // CUDA kernel Configuration
    dim3 dimGrid = dim3(ceil((int)numBColumns/(int)BLOCK_WIDTH),ceil((int)numARows/(int)BLOCK_WIDTH), 1);
    dim3 dimBlock = dim3(BLOCK_WIDTH, BLOCK_WIDTH, 1);

    // CUDA kernel for matrix multiplication
    StopWatchInterface* timer = NULL;
    sdkCreateTimer(&timer);
    sdkStartTimer(&timer);

    matMulGPU <<<dimGrid, dimBlock>>>(deviceA, deviceB, deviceC, numARows, numAColumns, numBColumns, numCColumns);

    sdkStopTimer(&timer);
    timeOnGPU = sdkGetTimerValue(&timer);
    sdkDeleteTimer(&timer);
    timer = NULL;

    // Copy data from device matrix into host matrix
    reuslt = cudaMemcpy(hostC, deviceC, sizeC, cudaMemcpyDeviceToHost);
    if(result != cudaSuccess)
    {
        printf("Device to Host data copy is failed for hostC matrix.\n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    // Matrix multiplication on host
    matMulCPU(hostA, hostB, gold, numARows, numAColumns, numBColumns, numCColumns);

    // Comparison
    int breakValue = -1;
    bool bAccuracy = true;
    for (int i = 0; i < numCRows * numCColumns; i++)
    {
        float val1 = gold[i];
        float val2 = hostC[i];
        if(val1 != val2)
        {
            bAccuracy = false;
            breakValue = i;
            break;
        }
    }

    char str[128];
    if(bAccuracy == false)
        sprintf(str, "Comparison of CPU and GPU matrix mult is not within accuracy of 0.000001 at array index %d", breakValue);
    else
        sprintf(str, "Comparison of CPU and GPU matrix mult is within accuracy of 0.000001");
    // Output
    printf("Time taken for Matrix Multiplication on CPU = %.6f\n", timeOnCPU);
    printf("Time taken for Matrix Multiplication on GPU = %.6f\n", timeOnGPU);
    printf("%s",str);

    // Cleanuop
    cleanup();

    return 0;
}

void InitA(int *data, int row, int col)
{
    int num = 1;
    // Code
    for(int i = 0; i < row; i++)
    {
        for(int j=0; j < col; j++)
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
    for(int i = 0; i < row; i++)
    {
        for(int j=0; j < col; j++)
        {
            *(data + i * col + j) = num;
            num--;
        }
    }
}

void matMulCPU(int* A, int* B, int* C, int numARows, int numAColumns, int numBColumns, int numCColumns)
{
    // Code
    StopWatchInterface* timer = NULL;
    sdkCreateTimer(&timer);
    sdkStartTimer(&timer);

    for (int i = 0; i < numARows; ++i)
    {
        for(int j = 0; j < numBColumns; ++j)
        {
            int value = 0.0f;
            for (int k = 0; k < numAColumns; ++k)
            {
                int a = A[i * numAColumns + k];
                int b = B[k * numBColumns + j];
                value += a*b;
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
        cudaFree(deviceC);
        deviceC = NULL;
    }
    if (deviceB)
    {
        cudaFree(deviceB);
        deviceB = NULL;
    }
    if (deviceA)
    {
        cudaFree(deviceA);
        deviceA = NULL;
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
