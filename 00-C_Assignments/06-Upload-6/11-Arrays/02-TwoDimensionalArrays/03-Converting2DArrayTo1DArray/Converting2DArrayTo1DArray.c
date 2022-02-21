#include <stdio.h>
#define NUM_ROWS 5
#define NUM_COLUMNS 3

int main(void)
{
    //variable declaraions
    int sab_iArray_2D[NUM_ROWS][NUM_COLUMNS];
    int sab_iArray_1D[NUM_ROWS * NUM_COLUMNS];
    int i, j;
    int sab_num;
    //code
    printf("Enter Elements Of Your Choice To Fill Up The Integer 2D Array : \n\n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        printf("For ROW NUMBER %d : \n", (i + 1));
        for (j = 0; j < NUM_COLUMNS; j++)
        {
            printf("Enter Element Number %d : \n", (j + 1));
            scanf("%d", &sab_num);
            sab_iArray_2D[i][j] = sab_num;
        }
        printf("\n\n");
    }

    printf("\n\n");
    printf("Two-Dimensional ( 2D ) Array Of Integers : \n\n");
    for (i = 0; i < NUM_ROWS; i++)
    {
        printf("****** ROW %d ******\n", (i + 1));
        for (j = 0; j < NUM_COLUMNS; j++)
        {
            printf("iArray_2D[%d][%d] = %d\n", i, j, sab_iArray_2D[i][j]);
        }
        printf("\n\n");
    }
    for (i = 0; i < NUM_ROWS; i++)
    {
        for (j = 0; j < NUM_COLUMNS; j++)
        {
            sab_iArray_1D[(i * NUM_COLUMNS) + j] = sab_iArray_2D[i][j];
        }
    }
    printf("\n\n");
    printf("One-Dimensional ( 1D ) Array Of Integers : \n\n");
    for (i = 0; i < (NUM_ROWS * NUM_COLUMNS); i++)
    {
        printf("iArray_1D[%d] = %d\n", i, sab_iArray_1D[i]);
    }
    printf("\n\n");
    return(0);
}
