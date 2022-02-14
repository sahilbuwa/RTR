#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main (void)
{
    //Function Prototypes
    int sab_MyStrlen(char[]);

    //Variable Declarations
    char sab_strArray[10][15] = {"Hello!", "Welcome", "To", "Real", "Time", "Rendering", "Batch", "(2020-21)", "Of", "ASTROMEDICOMP." };
    int sab_iStrLenghts[10] , sab_strArray_size , sab_strArray_num_rows , i , j;

    //code
    sab_strArray_size = sizeof(sab_strArray);
    sab_strArray_num_rows =sab_strArray_size / sizeof(sab_strArray[0]);

    for(i=0; i < sab_strArray_num_rows; i++ )
        sab_iStrLenghts[i] = sab_MyStrlen(sab_strArray[i]);

    printf("\n\n");
    printf("The Entire String Array : \n\n");
    for(i = 0; i <sab_strArray_num_rows;i++)
        printf("%s", sab_strArray[i]);

    printf("\n\n");
    printf("Strings in the 2D array : \n\n");

    for(i=0;i <sab_strArray_num_rows;i++)
    {
        printf("String number %d => %s\n\n",(i+1),sab_strArray[i]);
        for(j=0;j < sab_iStrLenghts[i];j++)
        {
            printf("Character %d = %c\n",(j+1),sab_strArray[i][j]);
        }
        printf("\n\n");

    }
    return 0;
    

}

int sab_MyStrlen(char sab_str[])
{
    //Variables
    int j , sab_string_length=0;

    //code
    for(j=0;j < MAX_STRING_LENGTH;j++)
    {
        if( sab_str[j] == '\0')
            break;
        else 
            sab_string_length++;

    }
    return sab_string_length;
}
