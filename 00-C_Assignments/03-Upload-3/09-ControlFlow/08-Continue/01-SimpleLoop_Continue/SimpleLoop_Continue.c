#include<stdio.h>
int main(void)
{
    //Variable Declarations
    int sab_i;

    //code
    printf("\n\n");

    printf("Printing Even Numbers From 0 to 100: \n\n");

    for(sab_i = 0;sab_i <= 100; sab_i++)
    {
        if ( sab_i % 2 != 0)
        {
            continue;
        }
        else
        {
            printf("\t%d\n",sab_i);
        }
    }
    printf("\n\n");
    return 0;
}