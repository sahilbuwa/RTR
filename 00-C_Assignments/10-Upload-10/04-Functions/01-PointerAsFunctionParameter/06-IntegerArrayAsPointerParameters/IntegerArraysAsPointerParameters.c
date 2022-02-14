#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[],char *envp[])
{
    // Function declarations
    void sab_MultiplyArrayElementsByNumber(int *, int, int);
    // Variable declaration
    int *sab_iArray = NULL,sab_num_elements,i,sab_num;

    // Code
    printf("\n\n");
    printf("Enter how many elements you want in the integer array: ");
    scanf("%d",&sab_num_elements);

    sab_iArray=(int *)malloc(sab_num_elements*sizeof(int));
    if(sab_iArray == NULL)
    {
        printf("Memory allocation to iArray has failed!!!Exiting now...\n\n");
        exit(0);
    }

    printf("\n\n");
    printf("Enter %d elements for the integer array: \n\n",sab_num_elements);
    for(i=0;i<sab_num_elements;i++)
        scanf("%d",&sab_iArray[i]);

    printf("\n\n");
    printf("Array before passing to function multiply array elements by number() :\n\n");
    for(i=0;i<sab_num_elements;i++)
        printf("iArray[%d] = %d\n",i,sab_iArray[i]);

    printf("\n\n");
    printf("Enter the number by which you want to multiply each array element: ");
    scanf("%d",&sab_num);

    sab_MultiplyArrayElementsByNumber(sab_iArray,sab_num_elements,sab_num);

    printf("\n\n");
    printf("Array eeturned by function MultiplyArrayElementsByNumber(): \n\n");
    for(i=0;i<sab_num_elements;i++)
        printf("iArray[%d] = %d\n",i,sab_iArray[i]);

    if(sab_iArray)
    {
        free(sab_iArray);
        sab_iArray=NULL;
        printf("\n\n");
        printf("Memory allocated to iArray has been freed!!!\n\n");
    }
    return 0;
}

void sab_MultiplyArrayElementsByNumber(int *sab_arr,int sab_iNumElements,int sab_n)
{
    // Variable declarations
    int i;
    // Code
    for(i=0;i<sab_iNumElements;i++)
        sab_arr[i]=sab_arr[i]*sab_n;

}
