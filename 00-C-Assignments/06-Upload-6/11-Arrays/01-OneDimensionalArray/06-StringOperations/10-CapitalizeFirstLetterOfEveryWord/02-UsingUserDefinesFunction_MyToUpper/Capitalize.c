#include <stdio.h>

#define MAX_STRING_LENGTH 512
#define SPACE ' '
#define FULLSTOP '.'
#define COMMA ','
#define EXCLAMATION '!'
#define QUESTION_MARK '?'

int main(void)
{
    //function prototype
    int sab_MyStrlen(char[]);
    char sab_MyToUpper(char);
    //variable declarations
    char sab_chArray[MAX_STRING_LENGTH], sab_chArray_CapitalizedFirstLetterOfEveryWord[MAX_STRING_LENGTH];
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
        if (i == 0) 
            sab_chArray_CapitalizedFirstLetterOfEveryWord[j] = sab_MyToUpper(sab_chArray[i]);

        else if (sab_chArray[i] == SPACE) 
        {
            sab_chArray_CapitalizedFirstLetterOfEveryWord[j] = sab_chArray[i];
            sab_chArray_CapitalizedFirstLetterOfEveryWord[j + 1] = sab_MyToUpper(sab_chArray[i + 1]);
            j++;
            i++;
        }
        else if ((sab_chArray[i] == FULLSTOP || sab_chArray[i] == COMMA || sab_chArray[i] == EXCLAMATION || sab_chArray[i] == QUESTION_MARK) && (sab_chArray[i] != SPACE))
        {
            sab_chArray_CapitalizedFirstLetterOfEveryWord[j] = sab_chArray[i];
            sab_chArray_CapitalizedFirstLetterOfEveryWord[j + 1] = SPACE;
            sab_chArray_CapitalizedFirstLetterOfEveryWord[j + 2] = sab_MyToUpper(sab_chArray[i + 1]);
            j = j + 2;
            i++;
        }
        else
            sab_chArray_CapitalizedFirstLetterOfEveryWord[j] = sab_chArray[i];

        j++;
    }
    sab_chArray_CapitalizedFirstLetterOfEveryWord[j] = '\0';
    
    printf("\n\n");
    printf("String Entered By You Is : \n\n");
    printf("%s\n", sab_chArray);
    printf("\n\n");
    printf("String After Capitalizing First Letter Of Every Word : \n\n");
    printf("%s\n", sab_chArray_CapitalizedFirstLetterOfEveryWord);

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


char sab_MyToUpper(char sab_ch)
{
    //variable declaration
    int sab_num;
    int c;
    //code
    sab_num = 'a' - 'A';
    if ((int)sab_ch >= 97 && (int)sab_ch <= 122)
    {
        c = (int)sab_ch - sab_num;
        return((char)c);
    }
    else
        return sab_ch;
}
