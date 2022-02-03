#include<stdio.h>
#include<conio.h>
int main(void)
{
    //Variable Declarations
    int sab_i,sab_j;

    //code
    printf("\n\n");

    for(sab_i = 1;sab_i <= 20; sab_i++)
    {
        for(sab_j = 1;sab_j <= 20; sab_j++)
        {
            if(sab_j > sab_i)
            {
                break;
            }
            else
            {
                printf("* ");
            }

        }
        printf("\n");
    }
    printf("\n\n");
    return 0;
}
