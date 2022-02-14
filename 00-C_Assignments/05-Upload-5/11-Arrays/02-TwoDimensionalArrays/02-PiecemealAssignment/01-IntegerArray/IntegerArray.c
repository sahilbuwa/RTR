#include<stdio.h>

int main(coid)
{
    int sab_iArray[3][5];
    int sab_int_size,sab_iArray_size,sab_iArray_num_elements,sab_iArray_num_rows,sab_iArray_num_columns;
    int i,j;

    //code
    printf("\n\n");

    sab_int_size = sizeof(int);

    sab_iArray_size = sizeof(sab_iArray);
    printf("Size of Two Dimensional (2D) Integer Array is = %d \n\n",sab_iArray_size);

    sab_iArray_num_rows = sab_iArray_size / sizeof(sab_iArray[0]);
    printf("Number of rows in two dimensional (2D) integer array is = %d \n\n",sab_iArray_num_rows);

    sab_iArray_num_columns = sizeof(sab_iArray[0]) / sab_int_size;
    printf("Number of columns in two dimensional (2D) integer array is = %d \n\n",sab_iArray_num_columns);

    sab_iArray_num_elements = sab_iArray_num_rows * sab_iArray_num_columns;
    printf("Number of elements in 2D integer array is %d\n\n",sab_iArray_num_elements);

    printf("\n\n");
    printf("Elements in the 2D array : \n\n");

    sab_iArray[0][0] = 21;
    sab_iArray[0][1] = 42;
    sab_iArray[0][2] = 63;
    sab_iArray[0][3] = 84;
    sab_iArray[0][4] = 105;

    sab_iArray[1][0] = 22;
    sab_iArray[1][1] = 44;
    sab_iArray[1][2] = 66;
    sab_iArray[1][3] = 88;
    sab_iArray[1][4] = 110;

    sab_iArray[2][0] = 23;
    sab_iArray[2][1] = 46;
    sab_iArray[2][2] = 69;
    sab_iArray[2][3] = 92;
    sab_iArray[2][4] = 115;

    for(i=0;i < sab_iArray_num_rows;i++)
    {
        printf("**********ROW %d *************\n",(i+1));
        for(j=0;j < sab_iArray_num_columns;j++)
        {
            printf("iArray[%d][%d] = %d\n",i,j,sab_iArray[i][j]);
        }
        printf("\n\n");
    }
    return 0;
}
