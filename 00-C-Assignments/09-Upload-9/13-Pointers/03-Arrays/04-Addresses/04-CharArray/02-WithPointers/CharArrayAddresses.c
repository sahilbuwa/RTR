#include <stdio.h>

int main(int argc,char *argv[],char *envp[])
{
    // Variable declarations
    char sab_cArray[10];
    char *sab_ptr_cArray = NULL;
    int i;
    // Code
    for (i = 0; i < 10; i++)
        sab_cArray[i] =(char)(i +65);

    sab_ptr_cArray = sab_cArray;
    printf("\n\n");
    printf("Elements of the character array: \n\n");

    for (i = 0; i < 10; i++)
        printf("cArray[%d] = %c\n", i,*(sab_ptr_cArray+i));

    printf("\n\n");
    printf("Elements of the character array: \n\n");

    for (i = 0; i < 10; i++)
        printf("cArray[%d] = %c\t \t address = %p\n", i,*(sab_ptr_cArray+i),(sab_ptr_cArray+i));
    
    printf("\n\n");
    
    return 0;
}
