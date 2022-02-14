#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(int argc,char *argv[],char *envp[])
{
    int i,j,**sab_ptr_iArray = NULL;
    // Code
    printf("\n\n");

    sab_ptr_iArray = (int **)malloc(NUM_ROWS*sizeof(int *));
    if(sab_ptr_iArray == NULL)
    {
        printf("Memory allocation of %d rows failed!!! Exiting now....\n\n",NUM_ROWS);
        exit(0);
    }
    else
        printf("Memory allocation of %d rows succeeded\n\n",NUM_ROWS);

    for(i=0;i<NUM_ROWS;i++)
    {
        sab_ptr_iArray[i] = (int *)malloc(NUM_COLUMNS*sizeof(int));
        if(sab_ptr_iArray == NULL)
        {
            printf("Memory allocation of %d rows failed!!! Exiting now....\n\n",i);
            exit(0);
        }
        else
            printf("Memory allocation of %d rows succeeded\n\n",i);
    }
    for(i=0;i<NUM_ROWS;i++)
    {
        for(j=0;j<NUM_COLUMNS;j++)
        {
            *(*(sab_ptr_iArray + i)+j) = (i+1)*(j+1);
        }
    }

    printf("\n\n");
    printf("2D integer array elements along with addresses : \n\n");
    for(i=0;i<NUM_ROWS;i++)
    {
        for(j=0;j<NUM_COLUMNS;j++)
        {
            printf("ptr_iArray_Row[%d][%d] = %d \t\t at address &ptr_iArray_Row[%d][%d] : %p\n",i,j,sab_ptr_iArray[i][j],i,j,&sab_ptr_iArray[i][j]);
        }
        printf("\n\n");
    }

    for(i=(NUM_ROWS-1);i>=0;i--)
    {
        if(*(sab_ptr_iArray + i))
        {
            free(*(sab_ptr_iArray+i));
            *(sab_ptr_iArray+i) = NULL;
            printf("Memory allocated to row %d has been freed!!! \n\n",i);
        }
    }
    if(sab_ptr_iArray)
    {
        free(sab_ptr_iArray);
        sab_ptr_iArray=NULL;
        printf("Memory allocated to ptr_iArray has been freed!!!\n\n");
    }
    return 0;

}
