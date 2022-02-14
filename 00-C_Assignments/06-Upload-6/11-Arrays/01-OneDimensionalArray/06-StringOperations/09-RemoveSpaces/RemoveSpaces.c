#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //function prototype
    int sab_MyStrlen(char[]);
    void sab_MyStrcpy(char[], char[]);
    //variable declarations
    char sab_chArray[MAX_STRING_LENGTH], sab_chArray_SpacesRemoved[MAX_STRING_LENGTH];
    int sab_iStringLength;
    int i, j;
    //code
    
    printf("\n\n");
    printf("Enter A String : \n\n");
    gets_s(sab_chArray, MAX_STRING_LENGTH);
    sab_iStringLength = sab_MyStrlen(sab_chArray);
    j = 0;
    for (i = 0; i < sab_iStringLength; i++)
    {
        if(sab_chArray[i] == ' ')
            continue;
        else
        {
            sab_chArray_SpacesRemoved[j] = sab_chArray[i];
            j++;
        }
    }
    sab_chArray_SpacesRemoved[j] = '\0';
    
    printf("\n\n");
    printf("String Entered By You Is : \n\n");
    printf("%s\n", sab_chArray);
    printf("\n\n");
    printf("String After Removal Of Spaces Is : \n\n");
    printf("%s\n", sab_chArray_SpacesRemoved);
    return(0);
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
