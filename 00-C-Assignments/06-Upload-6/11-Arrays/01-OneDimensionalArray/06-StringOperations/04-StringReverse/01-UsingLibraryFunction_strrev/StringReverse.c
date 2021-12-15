#include <stdio.h>
#include <string.h> 

#define MAX_STRING_LENGTH 512

int main(void)
{
    //variable declarations
    char sab_chArray_Original[MAX_STRING_LENGTH];
    //code
    
    printf("\n\n");
    printf("Enter A String : \n\n");
    gets_s(sab_chArray_Original, MAX_STRING_LENGTH);
    
    printf("\n\n");
    printf("The Original String Entered By You (i.e : 'chArray_Original[]') Is : \n\n");
    printf("%s\n", sab_chArray_Original);
    printf("\n\n");
    printf("The Reversed String (i.e : 'chArray_Reverse[]') Is : \n\n");
    printf("%s\n", strrev(sab_chArray_Original));
    return 0;
}
