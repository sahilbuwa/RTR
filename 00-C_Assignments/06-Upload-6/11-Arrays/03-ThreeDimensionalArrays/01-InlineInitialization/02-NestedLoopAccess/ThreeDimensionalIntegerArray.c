#include <stdio.h>

int main(void)
{
    //variable declaraions
    int sab_iArray[5][3][2] = { { { 9, 18 }, { 27, 36 }, { 45, 54 } },
                                { { 8, 16 }, { 24, 32 }, { 40, 48 } },
                                { { 7, 14 }, { 21, 28 }, { 35, 42 } },
                                { { 6, 12 }, { 18, 24 }, { 30, 36 } },
                                { { 5, 10 }, { 15, 20 }, { 25, 30 } } };
    int sab_int_size;
    int sab_iArray_size;
    int sab_iArray_num_elements, sab_iArray_width, sab_iArray_height, sab_iArray_depth;
    int i, j, k;
    //code
    printf("\n\n");
    sab_int_size = sizeof(int);
    sab_iArray_size = sizeof(sab_iArray);

    printf("Size Of Three Dimensional ( 3D ) Integer Array Is = %d\n\n", sab_iArray_size);

    sab_iArray_width = sab_iArray_size / sizeof(sab_iArray[0]);
    printf("Number of Rows (Width) In Three Dimensional ( 3D ) Integer Array Is = %d\n\n", sab_iArray_width);

    sab_iArray_height = sizeof(sab_iArray[0]) / sizeof(sab_iArray[0][0]);
    printf("Number of Columns (Height) In Three Dimensional ( 3D ) Integer Array Is = %d\n\n", sab_iArray_height);

    sab_iArray_depth = sizeof(sab_iArray[0][0]) / sab_int_size;
    printf("Depth In Three Dimensional ( 3D ) Integer Array Is = %d\n\n",sab_iArray_depth);

    sab_iArray_num_elements = sab_iArray_width * sab_iArray_height * sab_iArray_depth;
    printf("Number of Elements In Three Dimensional ( 3D ) Integer Array Is = %d\n\n", sab_iArray_num_elements);

    printf("\n\n");
    printf("Elements In Integer 3D Array : \n\n");
    for (i = 0; i < sab_iArray_width; i++)
    {
        printf("****** ROW %d ******\n", (i + 1));
        for (j = 0; j < sab_iArray_height; j++)
        {
            printf("****** COLUMN %d ******\n", (j + 1));
            for (k = 0; k < sab_iArray_depth; k++)
            {
                printf("iArray[%d][%d][%d] = %d\n", i, j, k, sab_iArray[i][j][k]);
            }
            printf("\n");
        }
        printf("\n\n");
    }
    return 0;
}
