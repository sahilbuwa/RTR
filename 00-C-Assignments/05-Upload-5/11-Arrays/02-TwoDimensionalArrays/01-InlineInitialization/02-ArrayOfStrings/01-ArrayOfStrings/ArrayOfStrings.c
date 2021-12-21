#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //function prototype
    int sab_MyStrlen(char[]);
    //variable declaraions
    char sab_strArray[10][15] = { "Hello!", "Welcome", "To", "Real", "Time", "Rendering", "Batch", "(2020-21)", "Of", "ASTROMEDICOMP." };
    int sab_char_size;
    int sab_strArray_size;
    int sab_strArray_num_elements, sab_strArray_num_rows, sab_strArray_num_columns;
    int sab_strActual_num_chars = 0;
    int i;
    //code
    printf("\n\n");
    sab_char_size = sizeof(char);
    sab_strArray_size = sizeof(sab_strArray);
    printf("Size Of Two Dimensional ( 2D ) Character Array (String Array) Is = %d \n\n", sab_strArray_size);
    sab_strArray_num_rows = sab_strArray_size / sizeof(sab_strArray[0]);
    printf("Number of Rows (Strings) In Two Dimensional ( 2D ) Character Array (String Array) Is = %d\n\n", sab_strArray_num_rows);
    sab_strArray_num_columns = sizeof(sab_strArray[0]) / sab_char_size;
    printf("Number of Columns In Two Dimensional ( 2D ) Character Array (String Array) Is = %d\n\n", sab_strArray_num_columns);
    sab_strArray_num_elements = sab_strArray_num_rows * sab_strArray_num_columns;
    printf("Maximum Number of Elements (Characters) In Two Dimensional ( 2D ) Character Array (String Array) Is = %d\n\n", sab_strArray_num_elements);
    for (i = 0; i < sab_strArray_num_rows; i++)
    {
        sab_strActual_num_chars = sab_strActual_num_chars + sab_MyStrlen(sab_strArray[i]);
    }
    printf("Actual Number of Elements (Characters) In Two Dimensional ( 2D ) Character Array (String Array) Is = %d\n\n", sab_strActual_num_chars);
    printf("\n\n");
    printf("Strings In The 2D Array : \n\n");
   
    printf("%s ", sab_strArray[0]);
    printf("%s ", sab_strArray[1]);
    printf("%s ", sab_strArray[2]);
    printf("%s ", sab_strArray[3]);
    printf("%s ", sab_strArray[4]);
    printf("%s ", sab_strArray[5]);
    printf("%s ", sab_strArray[6]);
    printf("%s ", sab_strArray[7]);
    printf("%s ", sab_strArray[8]);
    printf("%s\n\n", sab_strArray[9]);
    return 0;

}

int sab_MyStrlen(char sab_str[])
{
    //variable declarations
    int j;
    int sab_string_length = 0;
    //code
    for (j = 0; j < MAX_STRING_LENGTH; j++)
    {
        if (sab_str[j] == '\0')
            break;
        else
            sab_string_length++;
    }
    return sab_string_length;
}
