#include<stdio.h>

#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(int argc,char *argv[],char *envp[])
{
    // Variable Declarations
    int sab_iArray[NUM_ROWS][NUM_COLUMNS];
    int i,j;
    int *sab_ptr_iArray_row = NULL;

    // Code
    for(i=0;i<NUM_ROWS;i++)
    {
        sab_ptr_iArray_row = sab_iArray[i];
        for(j=0;j<NUM_COLUMNS;j++)
        {
            *(sab_ptr_iArray_row+j) = (i+1)*(j+1);
        }
    }
    printf("\n\n");
    printf("2D integer array elements along with addresses: \n\n");
    for(i=0;i<NUM_ROWS;i++)
    {
        sab_ptr_iArray_row = sab_iArray[i];
        for(j=0;j<NUM_COLUMNS;j++)
        {
            printf("*(ptr_iArray_row + %d) = %d \t\t at address (ptr_iArray_row + j ) : %p\n",j,*(sab_ptr_iArray_row+j),(sab_ptr_iArray_row+j));

        }
        printf("\n\n");
    }
    return 0;
}
