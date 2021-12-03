#include<stdio.h>
int main(void)
{
    //Variable Declarations
    int sab_a,sab_b,sab_p;

    //code
    sab_a=9;
    sab_b=30;
    sab_p=30;

    printf("\n\n");
    if (sab_a < sab_b)
    {
        printf("Entering First if-block...\n\n");
        printf("A Is Less Than B !!!\n\n");
    }
    else
    {
        printf("Entering First else-block...\n\n");
        printf("A Is NOT Less Than B !!!\n\n");
    }
        printf("First if-else Pair Done !!!\n\n");
        // *** SECOND if-else PAIR ***
        printf("\n\n");
    if (sab_b != sab_p)
    {
        printf("Entering Second if-block...\n\n");
        printf("B Is NOT Equal To P !!!\n\n");
    }
    else
    {
        printf("Entering Second else-block...\n\n");
        printf("B Is Equal To P !!!\n\n");
    }
    printf("Second if-else Pair Done !!!\n\n");
    return 0;
}