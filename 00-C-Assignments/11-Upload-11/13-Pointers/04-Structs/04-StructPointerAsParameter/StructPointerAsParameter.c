#include <stdio.h>

// Defining structs
struct sab_MyData
{
    int sab_i;
    float sab_f;
    double sab_d;
};

int main(int argc,char *argv[],char *envp[])
{
    // Function prototypes
    void sab_ChangeValues(struct sab_MyData *);

    // Variable declarations
    struct sab_MyData *sab_pData=NULL;

    // Code
    printf("\n\n");
    sab_pData=(struct sab_MyData *)malloc(sizeof(struct sab_MyData));
    if(sab_pData == NULL)
    {
        printf("Failed to allocate memory to struct !!Exiting now...\n\n");
        exit(0);
    }
    else
        printf("Successfully allocated memory to struct!!!\n\n");

    sab_pData->sab_i=80;
    sab_pData->sab_f=11.45f;
    sab_pData->sab_d=1.2595;


    printf("\n\n");
    printf("Data members of struct are : \n\n");

    printf("i = %d\n",sab_pData->sab_i);
    printf("f = %f\n",sab_pData->sab_f);
    printf("d = %lf\n",sab_pData->sab_d);

    sab_ChangeValues(sab_pData);

    printf("\n\n");
    printf("Data members of struct are : \n\n");
    printf("i = %d\n",sab_pData->sab_i);
    printf("f = %f\n",sab_pData->sab_f);
    printf("d = %lf\n",sab_pData->sab_d);

    if(sab_pData)
    {
        free(sab_pData);
        sab_pData=NULL;
        printf("Memory allocated to struct has been freed !!\n\n");
    }

    return 0;
}
void sab_ChangeValues(struct sab_MyData *sab_pParam_Data)
{
    //code
    sab_pParam_Data->sab_i=9;
    sab_pParam_Data->sab_f=8.2f;
    sab_pParam_Data->sab_d=6.1998;

}
