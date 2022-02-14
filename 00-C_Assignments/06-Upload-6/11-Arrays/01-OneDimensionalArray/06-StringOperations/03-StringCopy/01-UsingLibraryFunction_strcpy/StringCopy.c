#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //variable declarations
    char sab_chArray_Original[MAX_STRING_LENGTH], sab_chArray_Copy[MAX_STRING_LENGTH]; 
    //code
    printf("\n\n");
    printf("Enter A String : \n\n");
    gets_s(sab_chArray_Original, MAX_STRING_LENGTH);

    strcpy(sab_chArray_Copy, sab_chArray_Original);

    printf("\n\n");
    printf("The Original String Entered By You (i.e : 'chArray_Original[]') Is : \n\n");
    printf("%s\n", sab_chArray_Original);
    printf("\n\n");
    printf("The Copied String (i.e : 'chArray_Copy[]') Is : \n\n");
    printf("%s\n", sab_chArray_Copy);
    return 0;
}
