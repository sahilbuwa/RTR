#include <stdio.h>

// Defining Structs
struct sab_MyData
{
    int sab_i;
    float sab_f;
    double sab_d;
};

int main(int argc,char *argv[],char *envp[])
{
    // Variable declarations
    int sab_i_size,sab_f_size,sab_d_size,sab_struct_MyData_size,sab_pointer_to_struct_MyData_size;
    typedef struct sab_MyData* sab_MyDataPtr;
    sab_MyDataPtr sab_pData;
    // Code
    printf("\n\n");
    sab_pData=(sab_MyDataPtr)malloc(sizeof(struct sab_MyData));
    if(sab_pData==NULL)
    {
        printf("Failed to allocate memory !!!Exiting now....\n\n");
        exit(0);
    }
    else
        printf("Successfully allocated memory!!!!\n\n");
        
    sab_pData->sab_i=85;
    sab_pData->sab_f=11.45f;
    sab_pData->sab_d=1.2995;

    printf("\n\n");

    printf("Data members of struct are :\n\n");
    printf("i = %d\n",sab_pData->sab_i);
    printf("f = %f\n",sab_pData->sab_f);
    printf("d = %lf\n",sab_pData->sab_d);

    sab_i_size=sizeof(sab_pData->sab_i);
    sab_f_size=sizeof(sab_pData->sab_f);
    sab_d_size=sizeof(sab_pData->sab_d);

    printf("\n\n");

    printf("Sizes of data members of struct are : \n\n");
    printf("Size of i = %d bytes\n",sab_i_size);
    printf("Size of f = %d bytes\n",sab_f_size);
    printf("Size of d = %d bytes\n",sab_d_size);

    sab_struct_MyData_size=sizeof(struct sab_MyData);
    sab_pointer_to_struct_MyData_size=sizeof(sab_MyDataPtr);
    
    printf("\n\n");

    printf("Size of struct MyData: %d bytes\n\n",sab_struct_MyData_size);
    printf("Size of pointer to struct MyData: %d bytes\n\n",sab_pointer_to_struct_MyData_size);
    if(sab_pData)
    {
        free(sab_pData);
        sab_pData=NULL;
        printf("Memory allocated to struct is freed!!!!\n\n");
    }
    return 0;
}
