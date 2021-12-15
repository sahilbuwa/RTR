#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //function prototype
    int sab_MyStrlen(char[]);
    //variable declarations
    char sab_chArray[MAX_STRING_LENGTH]; 
    int sab_iStringLength;
    int sab_count_A = 0, sab_count_E = 0, sab_count_I = 0, sab_count_O = 0, sab_count_U = 0; 
    int i;
    //code
   
    printf("\n\n");
    printf("Enter A String : \n\n");
    gets_s(sab_chArray, MAX_STRING_LENGTH);
    
    printf("\n\n");
    printf("String Entered By You Is : \n\n");
    printf("%s\n", sab_chArray);
    sab_iStringLength = sab_MyStrlen(sab_chArray);
    for (i = 0; i < sab_iStringLength; i++)
    {
        switch (sab_chArray[i])
        {
            case 'A':
            case 'a':
                sab_count_A++;
                break;
            case 'E':
            case 'e':
                sab_count_E++;
                break;
            case 'I':
            case 'i':
                sab_count_I++;
                break;
            case 'O':
            case 'o':
                sab_count_O++;
                break;
            case 'U':
            case 'u':
                sab_count_U++;
                break;
            default:
                break;
        }
    }
    printf("\n\n");
    printf("In The String Entered By You, The Vowels And The Number Of Their Occurences Are Are Follows : \n\n");
    printf("'A' Has Occured = %d Times !!!\n\n", sab_count_A);
    printf("'E' Has Occured = %d Times !!!\n\n", sab_count_E);
    printf("'I' Has Occured = %d Times !!!\n\n", sab_count_I);
    printf("'O' Has Occured = %d Times !!!\n\n", sab_count_O);
    printf("'U' Has Occured = %d Times !!!\n\n", sab_count_U);
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
