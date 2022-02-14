#include<stdio.h>
int main(void)
{
    //Variable Declarations
    int sab_i,sab_j;

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
            sab_j++;
        }
        sab_i++;
        printf("\n\n");
    }
    return 0;
}
