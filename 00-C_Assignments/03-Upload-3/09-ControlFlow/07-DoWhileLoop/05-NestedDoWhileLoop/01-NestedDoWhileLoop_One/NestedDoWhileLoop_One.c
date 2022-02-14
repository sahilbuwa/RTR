#include<stdio.h>
int main(void)
{
    //Variable Declarations
    int sab_i,sab_j;

    //code
    printf("\n\n");
    sab_i=1;
    do 
    {
        printf("i = %d\n",sab_i);
        printf("-------\n\n");

        sab_j=1;
        do 
        {
            printf("\tj = %d\n",sab_j);
            sab_j++;
        }while(sab_j <= 5);
        sab_i++;
        printf("\n\n");
    }while(sab_i <= 10);
    return 0;
}
