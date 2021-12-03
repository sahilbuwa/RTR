#include<stdio.h>
int main(void)
{
    //Variable Declarations
    int sab_i,sab_j,sab_c;

    //code
    printf("\n\n");
    sab_i=0;
    while(sab_i <= 64)
    {
        sab_j=0;
        while(sab_j <= 64)
        {
            sab_c = ((sab_i & 0x8)==0) ^ ((sab_j & 0x8)==0);
            if(sab_c==0)
                printf("  ");
            if(sab_c==1)
                printf("*");
            sab_j++;
        }
        printf("\n\n");
        sab_i++;
    }
    return 0;
}
