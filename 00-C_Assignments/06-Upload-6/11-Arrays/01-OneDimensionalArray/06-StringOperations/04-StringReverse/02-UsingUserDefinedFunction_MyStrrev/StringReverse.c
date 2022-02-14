#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //function prototype
    void sab_MyStrrev(char[], char[]);
    //variable declarations
    char sab_chArray_Original[MAX_STRING_LENGTH], sab_chArray_Reversed[MAX_STRING_LENGTH]; 
    //code
    printf("\n\n");
    printf("Enter A String : \n\n");
    gets_s(sab_chArray_Original, MAX_STRING_LENGTH);
    
    sab_MyStrrev(sab_chArray_Reversed, sab_chArray_Original);
    
    printf("\n\n");
    printf("The Original String Entered By You (i.e : 'chArray_Original[]') Is : \n\n");
    printf("%s\n", sab_chArray_Original);
    printf("\n\n");
    printf("The Reversed String (i.e : 'chArray_Reversed[]') Is : \n\n");
    printf("%s\n", sab_chArray_Reversed);
    return 0;
}

void sab_MyStrrev(char sab_str_destination[], char sab_str_source[])
{
    //function prototype
    int sab_MyStrlen(char[]);
    //variable declarations
    int sab_iStringLength = 0;
    int i, j, sab_len;
    //code
    sab_iStringLength = sab_MyStrlen(sab_str_source);
    sab_len = sab_iStringLength - 1;
    for (i = 0, j = sab_len; i < sab_iStringLength, j >= 0; i++, j--)
    {
        sab_str_destination[i] = sab_str_source[j];
    }
    sab_str_destination[i] = '\0';
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
