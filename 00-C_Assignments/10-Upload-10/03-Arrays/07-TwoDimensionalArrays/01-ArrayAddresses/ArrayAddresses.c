#include <stdio.h>
#include <stdlib.h>
#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(int argc,char *argv[],char *envp[])
{
    // Variable declarations
    int sab_iArray[NUM_ROWS][NUM_COLUMNS];
    int i, j;
    //code
    for (i=0;i<NUM_ROWS;i++)
    {
        for (j=0;j<NUM_COLUMNS;j++)
            sab_iArray[i][j]=(i + 1)*(j + 1);
    }
    printf("\n\n");
    printf("2D integer array elements along with addresses: \n\n");
    for (i=0;i<NUM_ROWS;i++)
    {
        for (j = 0; j < NUM_COLUMNS; j++)
            printf("iArray[%d][%d] = %d \t \tat address: %p\n",i,j,sab_iArray[i][j],&sab_iArray[i][j]);
        
        printf("\n\n");
    }
    return 0;
}
