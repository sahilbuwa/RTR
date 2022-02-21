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
    sab_ch = getch();

    printf("\n\n");

    if((sab_ch=='A'||sab_ch=='a')||(sab_ch=='E'||sab_ch=='e')||(sab_ch=='I'||sab_ch=='i')||(sab_ch=='O'||sab_ch=='o')||(sab_ch=='U'||sab_ch=='u'))
        printf("Character \'%c\' Entered by you , is a vowel character from the english alphabet !!!\n\n",sab_ch);
    else
    {
        sab_ch_value=(int)sab_ch;
        if((sab_ch_value>=SAB_CHAR_ALPHABET_UPPER_CASE_BEGINNING && sab_ch_value <= SAB_CHAR_ALPHABET_UPPER_CASE_ENDING) || (sab_ch_value >= SAB_CHAR_ALPHABET_LOWER_CASE_BEGINNING && sab_ch_value <= SAB_CHAR_ALPHABET_LOWER_CASE_ENDING))
        {
            printf("Character \'%c\' Entered by you is a consonant character from english alphabet !!!\n\n",sab_ch);

        }
        else if (sab_ch_value >= SAB_CHAR_DIGIT_BEGINNING && sab_ch_value <= SAB_CHAR_DIGIT_ENDING)
        {
            printf("Character \'%c\' Entered by you is a Digit character !!!\n\n",sab_ch);

        }
        else
        {
            printf("Character \'%c\' Entered by you is a Special character !!!\n\n",sab_ch);
        }
    }
    return 0;

}