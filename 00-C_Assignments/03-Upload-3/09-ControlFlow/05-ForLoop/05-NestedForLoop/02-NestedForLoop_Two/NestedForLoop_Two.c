#include<stdio.h>
int main(void)
{
    //Variable Declarations
    int sab_i,sab_j,sab_k;

    //code
    printf("\n\n");
    for(sab_i=1;sab_i <= 10;sab_i++)
    {
        printf("i = %d\n",sab_i);
        printf("-------\n\n");
        for(sab_j = 1;sab_j <= 5;sab_j++)
        {
            printf("\tj = %d\n",sab_j);
            printf("\t---------\n\n");
            for(sab_k=1;sab_k <=3;sab_k++)
            {
                printf("\t\tk = %d\n",sab_k);
            }
            printf("\n\n");
        }
        printf("\n\n");
    }
    return 0;
}