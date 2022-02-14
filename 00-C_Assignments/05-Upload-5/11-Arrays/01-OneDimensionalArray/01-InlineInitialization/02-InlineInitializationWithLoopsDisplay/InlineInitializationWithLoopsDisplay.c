#include<stdio.h>

int main(void)
{
    //Variable Declarations
    int sab_iArray[] = {9,30,6,12,98,95,20,23,2,45};
    int sab_int_size,sab_iArray_size,sab_iArray_num_elements;

    float sab_fArray[] = {1.2f , 2.3f , 3.4f, 4.5f, 5.6f, 6.7f, 7.8f, 8.9f };
    int sab_float_size,sab_fArray_size,sab_fArray_num_elements;

    char sab_cArray[] = {'A','S','T','A'};
    int sab_char_size,sab_cArray_size,sab_cArray_num_elements;

    printf("\n\n");
    printf("In-Line Initialization and loop (for) display of elemetns of array : \n\n");
    sab_int_size = sizeof(int);
    sab_iArray_size = sizeof(sab_iArray);
    sab_iArray_num_elements =sab_iArray_size / sab_int_size;
    
    for (int i=0;i < sab_iArray_num_elements;i++)
        printf("iArray[%d] (Element %d) = %d\n", i , (i+1), sab_iArray[i]);
    printf("\n\n");
    printf("Size Of Data type 'int' = %d bytes\n",sab_int_size);
    printf("Number Of Elements In 'int' Array 'iArray[]' = %d Elements\n",sab_iArray_num_elements);
    printf("Size Of Array 'iArray[]' (%d Elements * %d Bytes) = %d Bytes\n\n",sab_iArray_num_elements, sab_int_size, sab_iArray_size);


    printf("\n\n");
    printf("In-line Initialization And Loop (while) Display Of Elements of Array'fArray[]': \n\n");
    sab_float_size = sizeof(float);

    sab_fArray_size = sizeof(sab_fArray);
    sab_fArray_num_elements = sab_fArray_size / sab_float_size;
    int sab_i = 0;
    while(sab_i < sab_fArray_num_elements)
    {
        printf("fArray[%d] (Element %d) = %f\n", sab_i, (sab_i + 1), sab_fArray[sab_i]);
        sab_i++;
    }
    printf("\n\n");
    printf("Size Of Data type 'float' = %d bytes\n",sab_float_size);
    printf("Number Of Elements In 'float' Array 'fArray[]' = %d Elements\n",sab_fArray_num_elements);
    printf("Size Of Array 'fArray[]' (%d Elements * %d Bytes) = %d Bytes\n\n",sab_fArray_num_elements, sab_float_size, sab_fArray_size);


    printf("\n\n");
    printf("In-line Initialization And Loop (do while) Display Of Elements of Array 'cArray[]': \n\n");
    sab_char_size = sizeof(char);
    sab_cArray_size = sizeof(sab_cArray);
    sab_cArray_num_elements = sab_cArray_size / sab_char_size;
    sab_i = 0;
    do
    {
        printf("cArray[%d] (Element %d) = %c\n", sab_i, (sab_i + 1), sab_cArray[sab_i]);
        sab_i++;
    }while (sab_i < sab_cArray_num_elements);
    printf("\n\n");
    printf("Size Of Data type 'char' = %d bytes\n",sab_char_size);
    printf("Number Of Elements In 'char' Array 'cArray[]' = %d Elements\n",sab_cArray_num_elements);
    printf("Size Of Array 'cArray[]' (%d Elements * %d Bytes) = %d Bytes\n\n",sab_cArray_num_elements, sab_char_size, sab_cArray_size);
    return 0;
}