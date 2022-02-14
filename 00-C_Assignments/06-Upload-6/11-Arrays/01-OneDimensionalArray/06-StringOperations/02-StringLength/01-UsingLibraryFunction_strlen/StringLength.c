#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void)
{
    //variable declarations
    char sab_chArray[MAX_STRING_LENGTH]; 
    int sab_iStringLength = 0;
    //code
    printf("\n\n");
    printf("Enter A String : \n\n");
    gets_s(sab_chArray, MAX_STRING_LENGTH);
    
    printf("\n\n");
    printf("String Entered By You Is : \n\n");
    printf("%s\n", sab_chArray);
    
    printf("\n\n");
    sab_iStringLength = strlen(sab_chArray);
    printf("Length Of String Is = %d Characters !!!\n\n", sab_iStringLength);
    return(0);
}
