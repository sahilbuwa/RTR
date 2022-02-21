#include<stdio.h>
#include<conio.h>

#define SAB_CHAR_ALPHABET_UPPER_CASE_BEGINNING 65
#define SAB_CHAR_ALPHABET_UPPER_CASE_ENDING 90

#define SAB_CHAR_ALPHABET_LOWER_CASE_BEGINNING 97
#define SAB_CHAR_ALPHABET_LOWER_CASE_ENDING 122

#define SAB_CHAR_DIGIT_BEGINNING 48
#define SAB_CHAR_DIGIT_ENDING 57

int main(void)
{
    char sab_ch;
    int sab_ch_value;

    //code
    printf("Enter Character : ");
    sab_ch=getch();

    printf("\n\n");

    switch(sab_ch)
    {
        //FALL THROUGH CONDITION
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
            printf("Character \'%c\' Entered by you , is a vowel character from the english alphabet !!!\n\n",sab_ch);
            break;
        default:
            sab_ch_value=(int)sab_ch;
        if((sab_ch_value>=SAB_CHAR_ALPHABET_UPPER_CASE_BEGINNING && sab_ch_value <= SAB_CHAR_ALPHABET_UPPER_CASE_ENDING) || (sab_ch_value >= SAB_CHAR_ALPHABET_LOWER_CASE_BEGINNING && sab_ch_value <= SAB_CHAR_ALPHABET_LOWER_CASE_ENDING))
        {
            printf("Character \'%c\' Entered by you is a consonant character from english alphabet !!!\n\n",sab_ch);

        }
        else if (sab_ch_value >= SAB_CHAR_DIGIT_BEGINNING && sab_ch_value <= SAB_CHAR_DIGIT_ENDING)
        {
            printf("Character \'%c\' Entered by you is a special character !!!\n\n",sab_ch);

        }
        break;
    }
    printf("Switch case block complete !!!\n\n");
    return 0;

}