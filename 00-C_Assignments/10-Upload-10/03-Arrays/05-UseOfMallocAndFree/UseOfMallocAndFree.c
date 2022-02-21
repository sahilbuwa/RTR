#include<stdio.h>
#include<stdlib.h>

int main(int argc,char *argv[],char *envp[])
{
    // Variable Declarations
    int *sab_ptr_iArray = NULL;
    unsigned int sab_intArrayLength=0;
    int i;

    // Code
    printf("\n\n");
    printf("Enter the number of elements you want in your integer array : ");
    scanf("%d",&sab_intArrayLength);

    sab_ptr_iArray = (int *)malloc(sizeof(int) * sab_intArrayLength);
    if(sab_ptr_iArray == NULL)
    {
        printf("\n\n");
        printf("Memory allocation for integer array has failed!!\n");
        exit(0);
    }
    else
    {
        printf("\n\n");
        printf("Memory allocation successful !!!\n\n");
        printf("Memory addresses from %p to %p have been allocated to integer array!!!\n\n",sab_ptr_iArray,(sab_ptr_iArray+(sab_intArrayLength-1)));
    }
    printf("\n\n");
    printf("Enter %d elements for the integer array : \n\n",sab_intArrayLength);
    for(i=0;i<sab_intArrayLength;i++)
        scanf("%d",(sab_ptr_iArray+i));
    
    printf("\n\n");
    printf("The integer array entered by you , consisting of %d elements : \n\n",sab_intArrayLength);
    for (i=0;i<sab_intArrayLength; i++)
        printf("ptr_iArray[%d] = %d \t \t at address &ptr_iArray[%d] : %p\n",i,sab_ptr_iArray[i],i,&sab_ptr_iArray[i]);
    
    printf("\n\n");

    for (i=0;i<sab_intArrayLength;i++)
        printf("*(ptr_iArray + %d) = %d \t \t at address (ptr_iArray + %d) : %p\n",i,*(sab_ptr_iArray + i),i,(sab_ptr_iArray + i));

    if (sab_ptr_iArray)
    {
        free(sab_ptr_iArray);
        sab_ptr_iArray = NULL;
        printf("\n\n");
        printf("Memory allocated for integer array has been succesfully freed !!!\n\n");
    }
    return 0;
}
