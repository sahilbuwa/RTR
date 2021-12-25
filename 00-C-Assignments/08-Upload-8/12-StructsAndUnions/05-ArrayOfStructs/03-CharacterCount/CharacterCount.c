#include<stdio.h>
#include<ctype.h>
#include<string.h>

#define MAX_STRING_LENGTH 1024

struct sab_CharacterCount
{
    char sab_ch;
    int sab_ch_count;

}sab_character_and_count[]={{'A',0},
                            {'B',0},
                            {'C',0},
                            {'D',0},
                            {'E',0},
                            {'F',0},
                            {'G',0},
                            {'H',0},
                            {'I',0},
                            {'J',0},
                            {'K',0},
                            {'L',0},
                            {'M',0},
                            {'N',0},
                            {'O',0},
                            {'P',0},
                            {'Q',0},
                            {'R',0},
                            {'S',0},
                            {'T',0},
                            {'U',0},
                            {'V',0},
                            {'W',0},
                            {'X',0},
                            {'Y',0},
                            {'Z',0} };

#define SIZE_OF_ENTIRE_ARRAY_OF_STRUCTS sizeof(sab_character_and_count)
#define SIZE_OF_ONE_STRUCT_FROM_THE_ARRAY_OF_STRUCTS sizeof(sab_character_and_count[0])
#define NUM_ELEMENTS_IN_ARRAY (SIZE_OF_ENTIRE_ARRAY_OF_STRUCTS/SIZE_OF_ONE_STRUCT_FROM_THE_ARRAY_OF_STRUCTS)

int main(int argc,char *argv[],char *envp[])
{
    //variable declarations
    char sab_str[MAX_STRING_LENGTH];
    int i, j, sab_actual_string_length = 0;

    //code
    //String input
    printf("\n\n");
    printf("Enter a string: \n\n");
    gets_s(sab_str, MAX_STRING_LENGTH);

    sab_actual_string_length=strlen(sab_str);

    printf("\n\n");
    printf("The string you have entered is: \n\n");
    printf("%s\n\n",sab_str);

    for (i = 0;i < sab_actual_string_length;i++)
    {
        for (j = 0;j < NUM_ELEMENTS_IN_ARRAY;j++)
        {
            sab_str[i] = toupper(sab_str[i]);

            if(sab_str[i] == sab_character_and_count[j].sab_ch) 
                sab_character_and_count[j].sab_ch_count++; 
        }
    }
    printf("\n\n");
    printf("The number of occurences of all characters from the alphabet are as follows: \n\n");
    for (i = 0;i < NUM_ELEMENTS_IN_ARRAY;i++)
    {
        printf("Character %c =%d\n",sab_character_and_count[i].sab_ch,sab_character_and_count[i].sab_ch_count);
    }

    printf("\n\n");

    return 0;
}
