#include<stdio.h>
int main(void)
{
    //Variable Declarations
    int sab_i,sab_j;

    //code
    printf("\n\n");

    printf("Outer Loop Prints Odd Numbers between 1 and 10. \n\n");
    printf("Inner Loop prints even numbers between 1 and 10 for every odd number printed by outer loop. \n\n");

    printf("Printing Even Numbers From 0 to 100: \n\n");

    for(sab_i = 0;sab_i <= 10; sab_i++)
    {
        if ( sab_i % 2 != 0)
        {
            printf("i = %d\n", sab_i);
            printf("------------------\n");
            for(sab_j = 1;sab_j <= 10 ; sab_j++)
            {
                if(sab_j % 2 == 0 )
                {
                    printf("\tj = %d\n",sab_j);
                }
                else
                {
                    continue;
                }
            }
            printf("\n\n");
        }
        else
        {
            continue;
        }
    }
    printf("\n\n");
    return 0;
}