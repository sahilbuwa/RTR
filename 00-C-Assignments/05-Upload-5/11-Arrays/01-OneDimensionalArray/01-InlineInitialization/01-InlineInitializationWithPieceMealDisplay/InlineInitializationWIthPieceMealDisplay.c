#include <stdio.h>

int main(void)

{
    //variable declaraions
    int sab_iArray[] = { 9, 30, 6, 12, 98, 95, 20, 23, 2, 45 };
    int sab_int_size;
    int sab_iArray_size;
    int sab_iArray_num_elements;
    float sab_fArray[] = { 1.2f, 2.3f, 3.4f, 4.5f, 5.6f, 6.7f, 7.8f, 8.9f };
    int sab_float_size;
    int sab_fArray_size;
    int sab_fArray_num_elements;
    char sab_cArray[] = { 'A', 'S', 'T', 'R', 'O', 'M', 'E', 'D', 'I', 'C', 'O', 'M', 'P' };
    int sab_char_size;
    int sab_cArray_size;
    int sab_cArray_num_elements;

    //code

    printf("\n\n");
    printf("In-line Initialization And Piece-meal Display Of Elements of Array'iArray[]': \n\n");
    printf("iArray[0] (1st Element) = %d\n", sab_iArray[0]);
    printf("iArray[1] (2nd Element) = %d\n", sab_iArray[1]);
    printf("iArray[2] (3rd Element) = %d\n", sab_iArray[2]);
    printf("iArray[3] (4th Element) = %d\n", sab_iArray[3]);
    printf("iArray[4] (5th Element) = %d\n", sab_iArray[4]);
    printf("iArray[5] (6th Element) = %d\n", sab_iArray[5]);
    printf("iArray[6] (7th Element) = %d\n", sab_iArray[6]);
    printf("iArray[7] (8th Element) = %d\n", sab_iArray[7]);
    printf("iArray[8] (9th Element) = %d\n", sab_iArray[8]);
    printf("iArray[9] (10th Element) = %d\n\n", sab_iArray[9]);

    sab_int_size = sizeof(int);
    sab_iArray_size = sizeof(sab_iArray);
    sab_iArray_num_elements = sab_iArray_size / sab_int_size;
    printf("Size Of Data type 'int' = %d bytes\n",sab_int_size);
    printf("Number Of Elements In 'int' Array 'iArray[]' = %d Elements\n",sab_iArray_num_elements);
    printf("Size Of Array 'iArray[]' (%d Elements * %d Bytes) = %d Bytes\n\n",sab_iArray_num_elements, sab_int_size, sab_iArray_size);
    // ****** fArray[] ******
    printf("\n\n");
    printf("In-line Initialization And Piece-meal Display Of Elements of Array'fArray[]': \n\n");
    printf("fArray[0] (1st Element) = %f\n", sab_fArray[0]);
    printf("fArray[1] (2nd Element) = %f\n", sab_fArray[1]);
    printf("fArray[2] (3rd Element) = %f\n", sab_fArray[2]);
    printf("fArray[3] (4th Element) = %f\n", sab_fArray[3]);
    printf("fArray[4] (5th Element) = %f\n", sab_fArray[4]);
    printf("fArray[5] (6th Element) = %f\n", sab_fArray[5]);
    printf("fArray[6] (7th Element) = %f\n", sab_fArray[6]);
    printf("fArray[7] (8th Element) = %f\n", sab_fArray[7]);
    printf("fArray[8] (9th Element) = %f\n", sab_fArray[8]);
    printf("fArray[9] (10th Element) = %f\n\n",sab_fArray[9]);
    sab_float_size = sizeof(float);
    sab_fArray_size = sizeof(sab_fArray);
    sab_fArray_num_elements = sab_fArray_size / sab_float_size;
    printf("Size Of Data type 'float' = %d bytes\n",sab_float_size);
    printf("Number Of Elements In 'float' Array 'fArray[]' = %d Elements\n",sab_fArray_num_elements);
    printf("Size Of Array 'fArray[]' (%d Elements * %d Bytes) = %d Bytes\n\n",
    sab_fArray_num_elements, sab_float_size, sab_fArray_size);

    // ****** cArray[] ******
    printf("\n\n");
    printf("In-line Initialization And Piece-meal Display Of Elements of Array 'cArray[]': \n\n");

    printf("cArray[0] (1st Element) = %c\n", sab_cArray[0]);
    printf("cArray[1] (2nd Element) = %c\n", sab_cArray[1]);
    printf("cArray[2] (3rd Element) = %c\n", sab_cArray[2]);
    printf("cArray[3] (4th Element) = %c\n", sab_cArray[3]);
    printf("cArray[4] (5th Element) = %c\n", sab_cArray[4]);
    printf("cArray[5] (6th Element) = %c\n", sab_cArray[5]);
    printf("cArray[6] (7th Element) = %c\n", sab_cArray[6]);
    printf("cArray[7] (8th Element) = %c\n", sab_cArray[7]);
    printf("cArray[8] (9th Element) = %c\n", sab_cArray[8]);
    printf("cArray[9] (10th Element) = %c\n", sab_cArray[9]);
    printf("cArray[10] (11th Element) = %c\n", sab_cArray[10]);
    printf("cArray[11] (12th Element) = %c\n", sab_cArray[11]);
    printf("cArray[12] (13th Element) = %c\n\n", sab_cArray[12]);
    sab_char_size = sizeof(char);
    sab_cArray_size = sizeof(sab_cArray);
    sab_cArray_num_elements = sab_cArray_size / sab_char_size;
    printf("Size Of Data type 'char' = %d bytes\n",sab_char_size);
    printf("Number Of Elements In 'char' Array 'cArray[]' = %d Elements\n",sab_cArray_num_elements);
    printf("Size Of Array 'cArray[]' (%d Elements * %d Bytes) = %d Bytes\n\n",sab_cArray_num_elements, sab_char_size, sab_cArray_size);
    return 0;

}
