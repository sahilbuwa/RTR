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
    printf("Depth In Three Dimensional ( 3D ) Integer Array Is = %d\n\n", sab_iArray_depth);
    sab_iArray_num_elements = sab_iArray_width * sab_iArray_height * sab_iArray_depth;
    printf("Number of Elements In Three Dimensional ( 3D ) Integer Array Is = %d \n\n", sab_iArray_num_elements);
    printf("\n\n");
    printf("Elements In Integer 3D Array : \n\n");

    printf("****** ROW 1 ******\n");
    printf("****** COLUMN 1 ******\n");
    printf("iArray[0][0][0] = %d\n", sab_iArray[0][0][0]);
    printf("iArray[0][0][1] = %d\n", sab_iArray[0][0][1]);
    printf("\n");

    printf("****** COLUMN 2 ******\n");
    printf("iArray[0][1][0] = %d\n", sab_iArray[0][1][0]);
    printf("iArray[0][1][1] = %d\n", sab_iArray[0][1][1]);
    printf("\n");

    printf("****** COLUMN 3 ******\n");
    printf("iArray[0][2][0] = %d\n", sab_iArray[0][2][0]);
    printf("iArray[0][2][1] = %d\n", sab_iArray[0][2][1]);
    printf("\n\n");
    
    printf("****** ROW 2 ******\n");

    printf("****** COLUMN 1 ******\n");
    printf("iArray[1][0][0] = %d\n", sab_iArray[1][0][0]);
    printf("iArray[1][0][1] = %d\n", sab_iArray[1][0][1]);
    printf("\n");

    printf("****** COLUMN 2 ******\n");
    printf("iArray[1][1][0] = %d\n", sab_iArray[1][1][0]);
    printf("iArray[1][1][1] = %d\n", sab_iArray[1][1][1]);
    printf("\n");

    printf("****** COLUMN 3 ******\n");
    printf("iArray[1][2][0] = %d\n", sab_iArray[1][2][0]);
    printf("iArray[1][2][1] = %d\n", sab_iArray[1][2][1]);
    printf("\n\n");

   
    printf("****** ROW 3 ******\n");

    printf("****** COLUMN 1 ******\n");
    printf("iArray[2][0][0] = %d\n", sab_iArray[2][0][0]);
    printf("iArray[2][0][1] = %d\n", sab_iArray[2][0][1]);
    printf("\n");

    printf("****** COLUMN 2 ******\n");
    printf("iArray[2][1][0] = %d\n", sab_iArray[2][1][0]);
    printf("iArray[2][1][1] = %d\n", sab_iArray[2][1][1]);
    printf("\n");

    printf("****** COLUMN 3 ******\n");
    printf("iArray[2][2][0] = %d\n", sab_iArray[2][2][0]);
    printf("iArray[2][2][1] = %d\n", sab_iArray[2][2][1]);
    printf("\n\n");

 
    printf("****** ROW 4 ******\n");

    printf("****** COLUMN 1 ******\n");
    printf("iArray[3][0][0] = %d\n", sab_iArray[3][0][0]);
    printf("iArray[3][0][1] = %d\n", sab_iArray[3][0][1]);
    printf("\n");

    printf("****** COLUMN 2 ******\n");
    printf("iArray[3][1][0] = %d\n", sab_iArray[3][1][0]);
    printf("iArray[3][1][1] = %d\n", sab_iArray[3][1][1]);
    printf("\n");

    printf("****** COLUMN 3 ******\n");
    printf("iArray[3][2][0] = %d\n", sab_iArray[3][2][0]);
    printf("iArray[3][2][1] = %d\n", sab_iArray[3][2][1]);
    printf("\n\n");


    printf("****** ROW 5 ******\n");

    printf("****** COLUMN 1 ******\n");
    printf("iArray[4][0][0] = %d\n", sab_iArray[4][0][0]);
    printf("iArray[4][0][1] = %d\n", sab_iArray[4][0][1]);
    printf("\n");

    printf("****** COLUMN 2 ******\n");
    printf("iArray[4][1][0] = %d\n", sab_iArray[4][1][0]);
    printf("iArray[4][1][1] = %d\n", sab_iArray[4][1][1]);
    printf("\n");

    printf("****** COLUMN 3 ******\n");
    printf("iArray[4][2][0] = %d\n", sab_iArray[4][2][0]);
    printf("iArray[4][2][1] = %d\n", sab_iArray[4][2][1]);
    printf("\n\n");

    return 0;
}
