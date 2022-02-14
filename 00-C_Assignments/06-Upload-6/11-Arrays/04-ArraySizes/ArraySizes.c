#include <stdio.h>

int main(void)
{
    // variable declaration
    int sab_iArray_One[5];
    int sab_iArray_Two[5][3];
    int sab_iArray_Three[100][100][5];
    int sab_num_rows_2D;
    int sab_num_columns_2D;
    int sab_num_rows_3D;
    int sab_num_columns_3D;
    int sab_depth_3D;
    // code
    printf("\n\n");

    printf("Size of 1-D integer array iArray_One = %lu\n", sizeof(sab_iArray_One));
    printf("Number of elements in 1-D integer array iArray_One = %lu\n", (sizeof(sab_iArray_One) / sizeof(int)));
    printf("\n\n");

    printf("Size of 2-D integer array iArray_Two = %lu\n", sizeof(sab_iArray_Two));
    printf("Number rows in 2-D integer array iArray_Two = %lu\n", (sizeof(sab_iArray_Two) / sizeof(sab_iArray_Two[0])));
    sab_num_rows_2D = (sizeof(sab_iArray_Two) / sizeof(sab_iArray_Two[0]));
    printf("Number of elements (columns) in each row in 2-D integer array iArray_Two = %lu\n", (sizeof(sab_iArray_Two[0]) / sizeof(sab_iArray_Two[0][0])));
    sab_num_columns_2D = (sizeof(sab_iArray_Two[0]) / sizeof(sab_iArray_Two[0][0]));
    printf("Number of elements in total in 2-D Array iArray_Two = %d\n",(sab_num_rows_2D * sab_num_columns_2D));
    printf("\n\n");
    printf("\n\n");

    printf("Size of 3-D integer array iArray_Three = %lu\n", sizeof(sab_iArray_Three));
    printf("Number rows in 3-D integer array iArray_Three = %lu\n", (sizeof(sab_iArray_Three) / sizeof(sab_iArray_Three[0])));
    sab_num_rows_3D = (sizeof(sab_iArray_Three) / sizeof(sab_iArray_Three[0]));
    printf("Number of elements (columns) in one row in 3-D integer array iArray_Three = %lu\n", (sizeof(sab_iArray_Three[0]) / sizeof(sab_iArray_Three[0][0])));
    sab_num_columns_3D = (sizeof(sab_iArray_Three[0]) / sizeof(sab_iArray_Three[0][0]));
    printf("Number of elements (depth) in one column in one row in 3-D integerarray iArray_Three = %lu\n", (sizeof(sab_iArray_Three[0][0]) / sizeof(sab_iArray_Three[0][0][0])));
    sab_depth_3D = (sizeof(sab_iArray_Three[0][0]) / sizeof(sab_iArray_Three[0][0][0]));
    printf("Number of elements in total in 3-D Array iArray_Three = %d\n",(sab_num_rows_3D * sab_num_columns_3D * sab_depth_3D));

    printf("\n\n");
    return 0;
}
