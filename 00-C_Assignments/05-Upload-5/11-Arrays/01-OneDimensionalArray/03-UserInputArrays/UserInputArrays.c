#include<stdio.h>

#define INT_ARRAY_NUM_ELEMENTS 5
#define FLOAT_ARRAY_NUM_ELEMENTS 3
#define CHAR_ARRAY_NUM_ELEMENTS 15

int main(void)
{
    //variable declarations
    int sab_iArray[INT_ARRAY_NUM_ELEMENTS];
    float sab_fArray[FLOAT_ARRAY_NUM_ELEMENTS];
    char sab_cArray[CHAR_ARRAY_NUM_ELEMENTS];
    int sab_i, sab_num;
    printf("\n\n");
    printf("Enter Elements For 'Integer' Array : \n");

    for (sab_i = 0; sab_i < INT_ARRAY_NUM_ELEMENTS; sab_i++)
        scanf("%d", &sab_iArray[sab_i]);

    printf("\n\n");
    printf("Enter Elements For 'Floating-Point' Array : \n");
    for (sab_i = 0; sab_i < FLOAT_ARRAY_NUM_ELEMENTS; sab_i++)
        scanf("%f", &sab_fArray[sab_i]);

    printf("\n\n");
    printf("Enter Elements For 'Character' Array : \n");
    for (sab_i = 0; sab_i < CHAR_ARRAY_NUM_ELEMENTS; sab_i++)
    {
        sab_cArray[sab_i] = getch();
        printf("%c\n", sab_cArray[sab_i]);
    }
    printf("\n\n");

    printf("Integer Array Entered By You : \n\n");
    for (sab_i = 0; sab_i < INT_ARRAY_NUM_ELEMENTS; sab_i++)
        printf("%d\n", sab_iArray[sab_i]);

    printf("\n\n");

    printf("Floating-Point Array Entered By You : \n\n");
    for (sab_i = 0; sab_i < FLOAT_ARRAY_NUM_ELEMENTS; sab_i++)
        printf("%f\n", sab_fArray[sab_i]);

    printf("\n\n");

    printf("Character Array Entered By You : \n\n");
    for (sab_i = 0; sab_i < CHAR_ARRAY_NUM_ELEMENTS; sab_i++)
        printf("%c\n", sab_cArray[sab_i]);
    return 0;
}   
