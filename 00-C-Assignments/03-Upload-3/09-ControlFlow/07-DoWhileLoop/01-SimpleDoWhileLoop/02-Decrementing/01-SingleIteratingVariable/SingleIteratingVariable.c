#include<stdio.h>
int main(void)
{
    //Variable Declarations
    int sab_i;

    //code
    printf("\n\n");

    printf("Printing Digits 10 to 1 : \n\n");

    sab_i=10;
    do 
    {
        printf("\t%d\n",sab_i);
        sab_i--;
    }while(sab_i>=1);
    printf("\n\n");
    return 0;
}