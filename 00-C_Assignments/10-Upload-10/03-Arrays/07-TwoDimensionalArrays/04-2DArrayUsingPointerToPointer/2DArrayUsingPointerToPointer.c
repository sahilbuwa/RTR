#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[],char *envp[])
{
    // Variable declarations
    int **sab_ptr_iArray = NULL; 
    int i, j,sab_num_rows,sab_num_columns;

    // Code

    printf("\n\n");
    printf("Enter number of rows: ");
    scanf("%d",&sab_num_rows);

    printf("\n\n");
    printf("Enter number of columns: ");
    scanf("%d",&sab_num_columns);

    printf("\n\n");
    printf("***** Memory allocation to 2D integer array******\n\n");
    sab_ptr_iArray=(int **)malloc(sab_num_rows*sizeof(int *));
    if(sab_ptr_iArray == NULL)
    {
        printf("Failed to allocate memory to %d rows!!Exiting now...\n\n",sab_num_rows);
        exit(0);
    }
    else
        printf("Memory allocation to %d rows of 2D array succeeded!!!\n\n",sab_num_rows);
    

    for(i=0;i<sab_num_rows;i++)
    {
        sab_ptr_iArray[i]=(int *)malloc(sab_num_columns*sizeof(int));
        if(sab_ptr_iArray == NULL)
        {
            printf("Failed to allocate memory to %d rows!!Exiting now...\n\n",i);
            exit(0);
        }
        else
            printf("Memory allocation to %d rows of 2D array succeeded!!!\n\n",i);
    }


    for(i=0;i<sab_num_rows;i++)
    {
        for(j=0;j<sab_num_columns;j++)
        {
            sab_ptr_iArray[i][j]=(i*1)+(j*1);
        }
    }

    for(i=0;i<sab_num_rows;i++)
    {
        printf("Base address of row %d:ptr_iArray[%d] = %p\t at address : %p\n",i,i,sab_ptr_iArray[i],&sab_ptr_iArray[i]);
    }

    printf("\n\n");
    for(i=0;i<sab_num_rows;i++)
    {
        for(j=0;j<sab_num_columns;j++)
        {
            printf("ptr_iArray[%d][%d] = %d \t at address: %p\n",i,j,sab_ptr_iArray[i][j],&sab_ptr_iArray[i][j]);
        }
        printf("\n\n");
    }

    for(i=(sab_num_rows-1);i>=0;i--)
    {
        if(sab_ptr_iArray[i])
        {
            free(sab_ptr_iArray[i]);
            sab_ptr_iArray[i]=NULL;
            printf("Memory allocated to row %d has been freed!!!\n\n",i);
        }
    }

    if(sab_ptr_iArray)
    {
        free(sab_ptr_iArray);
        sab_ptr_iArray=NULL;
        printf("Memory allocated to ptr_IArray has been freed!!!\n\n");
    }
    return 0;
}
