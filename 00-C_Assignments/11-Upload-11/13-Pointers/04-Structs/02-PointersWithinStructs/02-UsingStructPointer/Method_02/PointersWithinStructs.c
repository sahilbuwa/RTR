#include <stdio.h>

// Defining Structs
struct sab_MyData
{
    int *sab_ptr_i;
    int sab_i;
    float *sab_ptr_f;
    float sab_f;
    double *sab_ptr_d;
    double sab_d;
};

int main(int argc,char *argv[],char *envp[])
{
    // Variable declarations
    struct sab_MyData *sab_pData=NULL;

    // Code
    printf("\n\n");
    sab_pData=(struct sab_MyData *)malloc(sizeof(struct sab_MyData));
    if (sab_pData == NULL)
    {
        printf("Failed to allocate memory to strcut!!!Exiting now...\n\n");
        exit(0);
    }
    else
        printf("Successfully allocated memory to struct!!!\n\n");

    sab_pData->sab_i=90;
    sab_pData->sab_ptr_i=&sab_pData->sab_i;
    sab_pData->sab_f=11.45f;
    sab_pData->sab_ptr_f=&sab_pData->sab_f;
    sab_pData->sab_d=30.121995;
    sab_pData->sab_ptr_d=&sab_pData->sab_d;

    printf("\n\n");
    printf("i = %d\n", *(sab_pData->sab_ptr_i));
    printf("Address of i= %p\n", sab_pData->sab_ptr_i);
    printf("\n\n");
    printf("f = %f\n", *(sab_pData->sab_ptr_f));
    printf("Address of f= %p\n", sab_pData->sab_ptr_f);
    printf("\n\n"); 
    printf("d = %lf\n", *(sab_pData->sab_ptr_d));
    printf("Address of d = %p\n", sab_pData->sab_ptr_d);
    if(sab_pData)
    {
        free(sab_pData);
        sab_pData = NULL;
        printf("Memory allocated to struct successfully freed!!!\n\n");
    }
    return 0;
}
