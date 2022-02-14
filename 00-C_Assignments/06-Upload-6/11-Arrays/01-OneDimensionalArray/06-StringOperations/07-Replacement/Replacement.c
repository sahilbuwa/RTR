#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //function prototype
    int sab_MyStrlen(char[]);
    void sab_MyStrcpy(char[], char[]);
    //variable declarations
    char sab_chArray_Original[MAX_STRING_LENGTH], sab_chArray_VowelsReplaced[MAX_STRING_LENGTH];
    int sab_iStringLength;
    int i;
    //code
    printf("\n\n");
    printf("Enter A String : \n\n");
    gets_s(sab_chArray_Original, MAX_STRING_LENGTH);

    sab_MyStrcpy(sab_chArray_VowelsReplaced, sab_chArray_Original);
    sab_iStringLength = sab_MyStrlen(sab_chArray_VowelsReplaced);
    for (i = 0; i < sab_iStringLength; i++)
    {
        switch (sab_chArray_VowelsReplaced[i])
        {
            case 'A':
            case 'a':
            case 'E':
            case 'e':
            case 'I':
            case 'i':
            case 'O':
            case 'o':
            case 'U':
            case 'u':
                sab_chArray_VowelsReplaced[i] = '*';
                break;
            default:
                break;
        }
    }
    
    printf("\n\n");

    printf("String Entered By You Is : \n\n");
    printf("%s\n", sab_chArray_Original);

    printf("\n\n");
    printf("String After Replacement Of Vowels By * Is : \n\n");
    printf("%s\n", sab_chArray_VowelsReplaced);
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
