#include<stdio.h>

int main(void)
{
    // Function Declarations
    void PrintCUDADeviceProperties(void);

    // Code
    PrintCUDADeviceProperties();

}

void PrintCUDADeviceProperties(void)
{
    // Code
    printf("CUDA INFORMATION :\n");
    printf("==============================================================\n");
    cudaError_t ret_cuda_rt;
    int dev_count;
    ret_cuda_rt = cudaGetDeviceCount(&dev_count);
    if(ret_cuda_rt != cudaSuccess)
    {
        printf("CUDA Runtime API Error - cudaGetDeviceCount() failed due to %s\n",cudaGetErrorString(ret_cuda_rt));
    }
    else if (dev_count == 0)
    {
        printf("There is no cuda supported device on this system.\n");
        return;
    }
    else
    {
        printf("Total Number of cuda supporting GPU device/devices on this system : %d\n", dev_count);
        for(int i=0; i < dev_count; i++)
        {
            cudaDeviceProp dev_prop;
            int driverVersion = 0, runtimeVersion = 0;

            ret_cuda_rt = cudaGetDeviceProperties(&dev_prop, i);
            if(ret_cuda_rt != cudaSuccess)
            {
                printf("%s in %s at line %d\n", cudaGetErrorString(ret_cuda_rt), __FILE__, __LINE__);
                return;
            }
            printf("\n");
            cudaDriverGetVersion(&driverVersion);
            cudaRuntimeGetVersion(&runtimeVersion);
            printf("***************CUDA DRIVER AND RUNTIME INFORMATION***********\n");
            printf("=============================================================\n");
            printf("CUDA Driver Version             :%d.%d\n", driverVersion / 1000, (driverVersion % 100) / 10);
            printf("CUDA Runtime Version            :%d.%d\n", runtimeVersion / 1000, (runtimeVersion % 100) / 10);
            printf("\n");
            printf("=============================================================\n");
            printf("*************** GPU Device General Information **************\n");
            printf("=============================================================\n");
            printf("GPU Device Number               :%d\n",i);
            printf("GPU Device Name                 :%s\n", dev_prop.name);
            printf("GPU Device Compute Capability   :%d.%d\n", dev_prop.major, dev_prop.minor);
            printf("GPU Device Clock Rate           :%d\n", dev_prop.clockRate);
            printf("GPU Device Type                 : ");
            if(dev_prop.integrated)
                printf("Integrated (On-Board)\n");
            else
                printf("Discrete ( Card )\n");
            printf("\n");
            printf("********** GPU Device Memory Information***************\n");
            printf("=============================================================\n");
            printf("GPU Device Total Memory         : %.0f GB = %.0f MB = %llu Bytes\n",((float)dev_prop.totalGlobalMem / 1046576.0f) / 1024.0f, (float)dev_prop.totalGlobalMem / 1048576.0f, (unsigned long long) dev_prop.totalGlobalMem);
            printf("GPU Device Constant Memory      :%llu Bytes\n", (unsigned long long)dev_prop.totalConstMem);
            printf("GPU Device Shared Memory Per SMProcessor : %lu\n", (unsigned long)dev_prop.sharedMemPerBlock);
            printf("\n");
            printf("******* GPU DEVICE MULTIPROCESSOR INFORMATION *********\n");
            printf("=============================================================\n");
            printf("GPU Device Number Of SMProcessors   :%d\n", dev_prop.multiProcessorCount);
            printf("GPU Device Number Of Registers Per SMProcessor  :%d\n", dev_prop.regsPerBlock);
            printf("\n");
            printf("***************** GPU DEVICE THREAD INFORMATION*********\n");
            printf("=============================================================\n");
            printf("GPU Device Maximum Number Of Threads Per SMProcessor    : %d\n", dev_prop.maxThreadsPerMultiProcessor);
            printf("GPU Device Maximum Number Of Threads Per Block          :%d\n", dev_prop.maxThreadsPerBlock);
            printf("GPU Device Threads In Warp                              :%d\n", dev_prop.warpSize);
            printf("GPU Device Maximum Thread Dimensions                    :( %d, %d, %d)\n", dev_prop.maxThreadsDim[0], dev_prop.maxThreadsDim[1], dev_prop.maxThreadsDim[2]);
            printf("GPU Device Maximum Grid Dimension                       :( %d, %d, %d)\n", dev_prop.maxGridSize[0], dev_prop.maxGridSize[1], dev_prop.maxGridSize[2]);
            printf("\n");
            printf("**************** GPU DEVICE DRIVER INFORMATION *****************\n");
            printf("=============================================================\n");
            printf("GPU Device has ECC support                              :%s\n", dev_prop.ECCEnabled ? "Enabled" : "Disabled");
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)

            printf("GPU Device Driver Mode ( TCC or WDDM ) :%s\n", dev_prop.tccDriver ? "TCC (Tesla Compute Cluster Driver)" : "WDDM (Windows Display Driver Model )");
#endif
            printf("*****************************************************************************\n");
        }
    }
    
}
