#include<stdio.h>
int main(void)
{
    //Variable Declarations
    int sab_i;

    //code
    printf("\n\n");

    printf("Printing Digits 1 to 10 : \n\n");

    sab_i=1;
    do
    {
        printf("\t%d\n",sab_i);
        sab_i++;
    }while(sab_i <= 10);
    printf("\n\n");
    return 0;
}