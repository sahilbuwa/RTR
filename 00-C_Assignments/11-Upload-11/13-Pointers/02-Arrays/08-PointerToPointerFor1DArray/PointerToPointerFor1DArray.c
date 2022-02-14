#include<stdio.h>
#include<stdlib.h>

int main(int argc,char *argv[],char *envp[])
{
    void sab_MyAlloc(int **sab_ptr,unsigned int sab_numberOfElements);

    // Variable Declarations
    int *sab_piArray = NULL;
    unsigned int sab_num_elements;
    int i;

    // Code
    printf("\n\n");
    printf("How many elements you want in integer array ?\n\n");
    scanf("%u",&sab_num_elements);

    printf("\n\n");
    sab_MyAlloc(&sab_piArray,sab_num_elements);

    printf("Enter %u elements to fill up your integer array : \n\n",sab_num_elements);
    for(i=0;i<sab_num_elements;i++)
        scanf("%d",&sab_piArray[i]);

    printf("\n\n");
    printf("The %u elements entered by you in the integer array : \n\n",sab_num_elements);
    for(i=0;i<sab_num_elements;i++)
        printf("%u\n",sab_piArray[i]);

    printf("\n\n");
    if(sab_piArray)
    {
        free(sab_piArray);
        sab_piArray=NULL;
        printf("Memory allocated has been freed!!!\n\n");
    }
    return 0;
}

void sab_MyAlloc(int **sab_ptr,unsigned int sab_numberOfElements)
{
    // Code
    *sab_ptr = (int *)malloc(sab_numberOfElements*sizeof(int));
    if(*sab_ptr==NULL)
    {
        printf("Could not allocate memory!!!Exiting now...\n\n");
        exit(0);
    }
    printf("MyAlloc() has successfully allocated %lu bytes for array\n",(sab_numberOfElements*sizeof(int)));
}
