#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //function prototype
    int sab_MyStrlen(char[]);
    //variable declarations
    char sab_chArray[MAX_STRING_LENGTH];
    int sab_iStringLength = 0;
    //code
    printf("\n\n");
    printf("Enter A String : \n\n");
    gets_s(sab_chArray, MAX_STRING_LENGTH);
   
    printf("\n\n");
    printf("String Entered By You Is : \n\n");
    printf("%s\n", sab_chArray);
    
    printf("\n\n");
    sab_iStringLength = sab_MyStrlen(sab_chArray);
    printf("Length Of String Is = %d Characters !!!\n\n", sab_iStringLength);
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
