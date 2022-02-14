#include<stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //function prototype
    int sab_MyStrlen(char[]);
    void sab_MyStrcpy(char[],char[]);

    //Variable Declarations
    char sab_chArray[MAX_STRING_LENGTH];
    int sab_iStringLength;
    int i;
    int sab_word_count = 0 , sab_space_count = 0;

    //Code

    printf("\n\n");
    printf("Enter a string : \n\n");
    gets_s(sab_chArray,MAX_STRING_LENGTH);

    sab_iStringLength = sab_MyStrlen(sab_chArray);

    for(i=0;i < sab_iStringLength;i++)
    {
        switch (sab_chArray[i])
        {
            case 32:
                sab_space_count++;
                break;
            default:
                break;
        }
    }
    
    sab_word_count = sab_space_count + 1;
    printf("\n\n");
    printf("String Entered by you is : \n\n");
    printf("%s\n",sab_chArray);

    printf("\n\n");
    printf("Number of spaces in the input string = %d\n\n",sab_space_count);
    printf("Number of words in the input string = %d\n\n",sab_word_count);

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
