#include<stdio.h>
int main(void)
{
    //Variable Declarations
    int sab_i,sab_j;

    //code
    printf("\n\n");

    printf("Printing Digits 1 to 10 and 10 to 100 : \n\n");
    sab_i=1;
    sab_j=10;
    do
    {
        printf("\t %d \t %d\n",sab_i,sab_j);
        sab_i++;
        sab_j =sab_j + 10;
    }while(sab_i<=10,sab_j<=100);
    printf("\n\n");
    return 0;
}