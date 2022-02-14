#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //function prototype
    void sab_MyStrcat(char[], char[]);
    //variable declarations
    char sab_chArray_One[MAX_STRING_LENGTH], sab_chArray_Two[MAX_STRING_LENGTH];

    //code
    printf("\n\n");
    printf("Enter First String : \n\n");
    gets_s(sab_chArray_One, MAX_STRING_LENGTH);
    printf("\n\n");
    printf("Enter Second String : \n\n");
    gets_s(sab_chArray_Two, MAX_STRING_LENGTH);
    
    printf("\n\n");
    printf("****** BEFORE CONCATENATION ******");
    printf("\n\n");
    printf("The Original First String Entered By You (i.e : 'chArray_One[]') Is : \n\n");
    printf("%s\n", sab_chArray_One);
    printf("\n\n");
    printf("The Original Second String Entered By You (i.e : 'chArray_Two[]') Is : \n\n");
    printf("%s\n", sab_chArray_Two);
    sab_MyStrcat(sab_chArray_One, sab_chArray_Two);
    printf("\n\n");
    printf("****** AFTER CONCATENATION ******");
    printf("\n\n");
    printf("'chArray_One[]' Is : \n\n");
    printf("%s\n", sab_chArray_One);
    printf("\n\n");
    printf("'chArray_Two[]' Is : \n\n");
    printf("%s\n", sab_chArray_Two);
    return 0;
}

void sab_MyStrcat(char sab_str_destination[], char sab_str_source[])
{
    //function prototype
    int sab_MyStrlen(char[]);
    //variable declarations
    int sab_iStringLength_Source = 0, sab_iStringLength_Destination = 0;
    int i, j;
    //code
    sab_iStringLength_Source = sab_MyStrlen(sab_str_source);
    sab_iStringLength_Destination = sab_MyStrlen(sab_str_destination);

    for (i = sab_iStringLength_Destination, j = 0; j < sab_iStringLength_Source; i++, j++)
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
