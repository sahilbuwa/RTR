#include <stdio.h>
#include <stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS_ONE 3
#define NUM_COLUMNS_TWO 8

int main(int argc,char *argv[],char *envp[])
{
    // Variable declarations
    int *sab_iArray[NUM_ROWS];
    int i, j;
    // Code
    printf("\n\n");
    printf("******* First Memory Allocation To 2D integer array ********\n\n");
    for(i=0;i<NUM_ROWS;i++)
    {
        sab_iArray[i]=(int *)malloc(NUM_COLUMNS_ONE*sizeof(int));
        if(sab_iArray[i]==NULL)
        {
            printf("Failed to allocate memory to row %d integer array!!! Exiting now...\n\n",i);
            exit(0);
        }
        else
            printf("Memory allocation of %d succeeded !!!\n\n",i);
    }


    for(i=0;i<NUM_ROWS;i++)
    {
        for (j=0;j<NUM_COLUMNS_ONE;j++)
        {
            sab_iArray[i][j]=(i+1)*(j+1);
        }
    }
    

    printf("\n\n");
    printf("DISPLAYING 2D ARRAY : \n\n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMNS_ONE; j++)
        {
            printf("iArray[%d][%d] = %d\n", i, j,sab_iArray[i][j]);
        }
        printf("\n\n");
    }
    printf("\n\n");


    for(i=(NUM_ROWS-1);i>=0;i--)
    {
        free(sab_iArray[i]);
        sab_iArray[i] = NULL;
        printf("Memory allocated to row %d has been freed !!!\n\n",i);
    }
    

    printf("\n\n");
    printf("*******Second Memory allocation to 2D integer array*******\n\n");
    for(i=0;i<NUM_ROWS;i++)
    {
        sab_iArray[i]=(int *)malloc(NUM_COLUMNS_TWO*sizeof(int));
        if(sab_iArray[i] == NULL)
        {
            printf("Failed to allocate memory to %d row !!!!Exiting now...\n\n",i);
            exit(0);
        }
        else
            printf("Memory allocation to row %d succeeded!!!\n\n",i);
    }
    for(i=0;i<NUM_ROWS;i++)
    {
        for(j=0;j<NUM_COLUMNS_TWO;j++)
        {
            sab_iArray[i][j]=(i+1)*(j+1);
        }
    }


    printf("\n\n");
    printf("Display 2d array : \n\n");
    for(i=0;i<NUM_ROWS;i++)
    {
        for(j=0;j<NUM_COLUMNS_TWO;j++)
        {
            printf("iArray[%d][%d] = %d\n",i,j,sab_iArray[i][j]);
        }
        printf("\n\n");
    }
    printf("\n\n");
    
    
    for(i=(NUM_ROWS-1);i>=0;i--)
    {
        free(sab_iArray[i]);
        sab_iArray[i] = NULL;
        printf("Memory allocated to row %d of 2D integer array has been freed!!!\n\n", i);
    }
    return 0;
}
