#include <stdio.h>
#include <stdlib.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 5

int main(int argc,char *argv[],char *envp[])
{
    // Variable declarations
    int *sab_iArray[NUM_ROWS];
    int i, j;
    // Code
    printf("\n\n");
    printf("******Memory allocation to 2D array********\n\n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        sab_iArray[i]=(int *)malloc((NUM_COLUMNS-i)*sizeof(int));
        if(sab_iArray[i]==NULL)
        {
            printf("Failed to allocate memory to row %d!!!Exiting now...\n\n", i);
            exit(0);
        }
        else
            printf("Memory allocation to row %d succeeded!!!\n\n", i);
    }

    for(i=0;i<5;i++)
    {
        for(j=0;j<(NUM_COLUMNS-i);j++)
        {
            sab_iArray[i][j]=(i*1)+(j*1);
        }
    }

    for(i=0;i<5;i++)
    {
        for(j=0;j<(NUM_COLUMNS-i);j++)
        {
            printf("iArray[%d][%d] = %d\t at address : %p\n",i,j,sab_iArray[i][j],&sab_iArray[i][j]);
        }
        printf("\n\n");
    }
    for(i=(NUM_ROWS-1);i>=0;i--)
    {
        if(sab_iArray[i])
        {
            free(sab_iArray[i]);
            sab_iArray[i] = NULL;
            printf("Memory allocated to row %d has been freed!!!\n\n",i);
        }
    }
    return 0;
}
