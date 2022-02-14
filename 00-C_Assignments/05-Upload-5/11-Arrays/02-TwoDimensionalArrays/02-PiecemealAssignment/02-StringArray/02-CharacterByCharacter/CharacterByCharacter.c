#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //Variables
    char sab_strArray[5][10];
    int sab_char_size,sab_strArray_size,sab_strArray_num_elements, sab_strArray_num_rows, sab_strArray_num_columns;
    int i;

    //Code
    printf("\n\n");

    sab_char_size = sizeof(char);

    sab_strArray_size = sizeof(sab_strArray);
    printf("Size of 2D character Array is =%d \n\n",sab_strArray_size);

    sab_strArray_num_rows = sab_strArray_size / sizeof(sab_strArray[0]);
    printf("Number of rows in 2D character array is = %d\n\n",sab_strArray_num_rows);

    sab_strArray_num_columns =sizeof(sab_strArray[0]) / sab_char_size;
    printf("Number of columns in 2D character array is = %d\n\n",sab_strArray_num_columns);

    sab_strArray_num_elements = sab_strArray_num_rows * sab_strArray_num_columns;
    printf("Maximum numbers of elements in 2D character array is %d\n\n",sab_strArray_num_elements);

    sab_strArray[0][0]='M';
    sab_strArray[0][1]='y';
    sab_strArray[0][2]='\0';

    sab_strArray[1][0]='N';
    sab_strArray[1][1]='a';
    sab_strArray[1][2]='m';
    sab_strArray[1][3]='e';
    sab_strArray[1][4]='\0';

    sab_strArray[2][0]='I';
    sab_strArray[2][1]='s';
    sab_strArray[2][2]='\0';

    sab_strArray[3][0]='S';
    sab_strArray[3][1]='a';
    sab_strArray[3][2]='h';
    sab_strArray[3][3]='i';
    sab_strArray[3][4]='l';
    sab_strArray[3][5]='\0';

    sab_strArray[4][0]='B';
    sab_strArray[4][1]='u';
    sab_strArray[4][2]='w';
    sab_strArray[4][3]='a';
    sab_strArray[4][4]='\0';

    printf("\n\n");
    printf("The strings in the 2D character array are : \n\n");

    for(i=0;i < sab_strArray_num_rows;i++)
        printf("%s",sab_strArray[i]);

    printf("\n\n");

    return 0;

}

int sab_MyStrlen(char sab_str[])
{
    int j,sab_string_length = 0;
    //Code
    for(j=0; j < MAX_STRING_LENGTH;j++)
    {
        if(sab_str[j]=='\0')
            break;
        else    
            sab_string_length++;

    }
    return sab_string_length;
}
