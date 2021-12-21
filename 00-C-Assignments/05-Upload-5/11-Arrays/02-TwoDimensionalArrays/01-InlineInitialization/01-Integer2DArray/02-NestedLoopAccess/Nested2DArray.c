#include <stdio.h>

int main(void)
{
    //variable declaraions
    int sab_iArray[5][3] = { {1, 2, 3}, {2, 4, 6}, {3, 6, 9}, {4, 8, 12}, {5, 10,15} };
    int sab_int_size;
    int sab_iArray_size;
    int sab_iArray_num_elements, sab_iArray_num_rows, sab_iArray_num_columns;
    int i, j;
    //code
    printf("\n\n");
    sab_int_size = sizeof(int);
    sab_iArray_size = sizeof(sab_iArray);
    printf("Size Of Two Dimensional ( 2D ) Integer Array Is = %d\n\n",sab_iArray_size);
    sab_iArray_num_rows = sab_iArray_size / sizeof(sab_iArray[0]);
    printf("Number of Rows In Two Dimensional ( 2D ) Integer Array Is = %d\n\n",sab_iArray_num_rows);
    sab_iArray_num_columns = sizeof(sab_iArray[0]) / sab_int_size;
    printf("Number of Columns In Two Dimensional ( 2D ) Integer Array Is = %d\n\n", sab_iArray_num_columns);
    sab_iArray_num_elements = sab_iArray_num_rows * sab_iArray_num_columns;
    printf("Number of Elements In Two Dimensional ( 2D ) Integer Array Is = %d\n\n", sab_iArray_num_elements);
    printf("\n\n");
    printf("Elements In The 2D Array : \n\n");
    
    for (i = 0; i < sab_iArray_num_rows; i++)
    {
        printf("****** ROW %d ******\n", (i + 1));
        for (j = 0; j < sab_iArray_num_columns; j++)
        {
            printf("iArray[%d][%d] = %d\n", i, j, sab_iArray[i][j]);
        }
        printf("\n\n");
    }
    return 0;
}
