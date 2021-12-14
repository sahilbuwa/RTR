#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //function declarations
    void sab_MyStrcpy(char[],char[]);

    //Variable Declarations
    char sab_strArray[5][10];
    int sab_char_size,sab_strArray_size,sab_strArray_num_elements,sab_strArray_num_rows,sab_strArray_num_columns;
    int i;

    //code
    printf("\n\n");

    sab_char_size = sizeof(char);

    sab_strArray_size = sizeof(sab_strArray);
    printf("Size of 2 dimensional 2D character array is = %d\n\n",sab_strArray_size);

    sab_strArray_num_rows = sab_strArray_size / sizeof(sab_strArray[0]);
    printf("Number of rows in 2D character array is = %d\n\n",sab_strArray_num_rows);

    sab_strArray_num_columns = sizeof(sab_strArray[0]) / sab_char_size;
    printf("Number of columns in 2D character Array is = %d\n\n",sab_strArray_num_columns);

    sab_strArray_num_elements = sab_strArray_num_rows * sab_strArray_num_columns;
    printf("Maximum number of elements in 2D character array is %d \n\n",sab_strArray_num_elements);

    sab_MyStrcpy(sab_strArray[0],"My");
    sab_MyStrcpy(sab_strArray[1],"Name");
    sab_MyStrcpy(sab_strArray[2],"is");
    sab_MyStrcpy(sab_strArray[3],"Sahil");
    sab_MyStrcpy(sab_strArray[4],"Buwa");

    printf("\n\n");
    printf("The strings in the 2D character array are : \n\n");

    for(i=0;i<sab_strArray_num_rows;i++)
        printf("%s",sab_strArray[i]);

    printf("\n\n");

    return 0;
}

void sab_MyStrcpy(char sab_str_destination[], char sab_str_source[])
{
    //function prototype
    int sab_MyStrlen(char[]);
    //variable declarations
    int sab_iStringLength = 0;
    int j;
    //code
    sab_iStringLength = sab_MyStrlen(sab_str_source);
    for (j = 0; j < sab_iStringLength; j++)
        sab_str_destination[j] = sab_str_source[j];

    sab_str_destination[j] = '\0';
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
    return(sab_string_length);
}
