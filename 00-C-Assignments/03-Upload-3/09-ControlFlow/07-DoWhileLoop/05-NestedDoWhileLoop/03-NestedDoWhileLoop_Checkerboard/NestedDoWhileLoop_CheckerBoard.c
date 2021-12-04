#include<stdio.h>
int main(void)
{
    //Variable Declarations
    int sab_i,sab_j,sab_c;

    //code
    printf("\n\n");
    sab_i=0;
    do 
    {
        sab_j=0;
        do 
        {
            sab_c = ((sab_i & 0x8)==0) ^ ((sab_j & 0x8)==0);
            if(sab_c==0)
                printf("  ");
            if(sab_c==1)
                printf("*");
            sab_j++;
        }while(sab_j <= 64);
        printf("\n\n");
        sab_i++;
    }while(sab_i <= 64);
    return 0;
}
