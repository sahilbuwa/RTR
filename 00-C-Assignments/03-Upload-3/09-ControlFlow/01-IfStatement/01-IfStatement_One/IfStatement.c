#include<stdio.h>
int main (void)
{
    //Variable Declarations
    int sab_a, sab_b ,sab_p;

    //code
    sab_a=9;
    sab_b=30;
    sab_p=31;

    printf("\n\n");
    if(sab_a<sab_b)
    {
        printf("A is less than B !!!\n\n");
    }
    if (sab_b != sab_p)
    {
        printf("B is not equal to P !!!\n\n");
    }
    
    printf("Both comparisions have been done!!!\n\n");
    return 0;
}
