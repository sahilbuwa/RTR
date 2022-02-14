#include<stdio.h>
int main(void)
{
    //Variable Declarations
    int sab_i,sab_j,sab_k;

    //code
    printf("\n\n");
    sab_i=1;
    while(sab_i <= 10)
    {
        printf("i = %d\n",sab_i);
        printf("-------\n\n");

        sab_j=1;
        while(sab_j <= 5)
        {
            printf("\tj = %d\n",sab_j);
            printf("\t---------\n\n");

            sab_k=1;
            while(sab_k <=3)
            {
                printf("\t\tk = %d\n",sab_k);
                sab_k++;
            }
            printf("\n\n");
            sab_j++;
        }
        printf("\n\n");
        sab_i++;
    }
    return 0;
}
