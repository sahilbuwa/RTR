#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //function prototype
    void sab_MyStrcpy(char[], char[]);
    //variable declarations
    char sab_chArray_Original[MAX_STRING_LENGTH], sab_chArray_Copy[MAX_STRING_LENGTH];
    
    printf("\n\n");
    printf("Enter A String : \n\n");
    gets_s(sab_chArray_Original, MAX_STRING_LENGTH);
    
    sab_MyStrcpy(sab_chArray_Copy, sab_chArray_Original);
    
    printf("\n\n");
    printf("The Original String Entered By You (i.e : 'chArray_Original[]') Is : \n\n");
    printf("%s\n", sab_chArray_Original);
    printf("\n\n");
    printf("The Copied String (i.e : 'chArray_Copy[]') Is : \n\n");
    printf("%s\n", sab_chArray_Copy);
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
    return sab_string_length;
}
