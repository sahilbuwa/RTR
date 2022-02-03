#include <stdio.h>

int main(int argc,char *argv[],char *envp[])
{
    // Variable declarations
    int sab_iArray[10];
    int *sab_ptr_iArray = NULL;
    int i;
    // Code
    for (i = 0; i < 10; i++)
        sab_iArray[i] = (i + 1) * 3;

    sab_ptr_iArray = sab_iArray;
    printf("\n\n");
    printf("Elements of the integer array: \n\n");

    for (i = 0; i < 10; i++)
        printf("iArray[%d] = %d\n", i,*(sab_ptr_iArray+i));

    printf("\n\n");
    printf("Elements of the integer array: \n\n");

    for (i = 0; i < 10; i++)
        printf("iArray[%d] = %d \t \t address = %p\n", i,*(sab_ptr_iArray+i),(sab_ptr_iArray+i));
    
    printf("\n\n");
    
    return 0;
}
