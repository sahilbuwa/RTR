#include <stdio.h>
int main(void)
{
    //variable declaraions
    int sab_iArray[5][3] = { {1, 2, 3}, {2, 4, 6}, {3, 6, 9}, {4, 8, 12}, {5, 10, 15} }; 
    int sab_int_size;
    int sab_iArray_size;
    int sab_iArray_num_elements, sab_iArray_num_rows, sab_iArray_num_columns;

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
    
    // *** ROW 1 ***
    printf("****** ROW 1 ******\n");
    printf("iArray[0][0] = %d\n", sab_iArray[0][0]); 
    printf("iArray[0][1] = %d\n", sab_iArray[0][1]);
    printf("iArray[0][2] = %d\n", sab_iArray[0][2]); 
    printf("\n\n");
    // *** ROW 2 ***
    printf("****** ROW 2 ******\n");
    printf("iArray[1][0] = %d\n", sab_iArray[1][0]); 
    printf("iArray[1][1] = %d\n", sab_iArray[1][1]); 
    printf("iArray[1][2] = %d\n", sab_iArray[1][2]); 
    printf("\n\n");
    // *** ROW 3 ***
    printf("****** ROW 3 ******\n");
    printf("iArray[2][0] = %d\n", sab_iArray[2][0]); 
    printf("iArray[2][1] = %d\n", sab_iArray[2][1]); 
    printf("iArray[2][2] = %d\n", sab_iArray[2][2]); 
    printf("\n\n");
    // *** ROW 4 ***
    printf("****** ROW 4 ******\n");
    printf("iArray[3][0] = %d\n", sab_iArray[3][0]);
    printf("iArray[3][1] = %d\n", sab_iArray[3][1]); 
    printf("iArray[3][2] = %d\n", sab_iArray[3][2]); 
    printf("\n\n");
    // *** ROW 5 ***
    printf("****** ROW 5 ******\n");
    printf("iArray[4][0] = %d\n", sab_iArray[4][0]); 
    printf("iArray[4][1] = %d\n", sab_iArray[4][1]); 
    printf("iArray[4][2] = %d\n", sab_iArray[4][2]); 
    printf("\n\n");
    return 0;

}
