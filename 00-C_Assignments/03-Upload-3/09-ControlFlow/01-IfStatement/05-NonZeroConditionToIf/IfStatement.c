#include<stdio.h>
int main(void)
{
    //Variable Declarations
    int sab_a;

    //code
    printf("\n\n");

    sab_a=5;
    if(sab_a)
    {
        printf("If block 1 : 'A' exists and has value = %d !!!\n\n",sab_a);

    }
    sab_a=-5;
    if(sab_a)
    {
        printf("If block 2 : 'A' exists and has value = %d !!!\n\n",sab_a);

    }
    sab_a=0;
    if(sab_a)
    {
        printf("If block 3 : 'A' exists and has value = %d !!!\n\n",sab_a);
    }
    printf("All three if statements are done !!!\n\n");
    return 0;
}