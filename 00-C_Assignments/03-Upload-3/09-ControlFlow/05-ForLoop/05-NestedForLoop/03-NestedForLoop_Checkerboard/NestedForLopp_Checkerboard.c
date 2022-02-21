#include<stdio.h>
int main(void)
{
    //Variable Declarations
    int sab_i,sab_j,sab_c;

    //code
    printf("\n\n");
    for(sab_i=1;sab_i < 64;sab_i++)
    {
        
        for(sab_j = 1;sab_j < 64;sab_j++)
        {
            sab_c = ((sab_i & 0x8)==0) ^ ((sab_j & 0x8)==0);
            if(sab_c==0)
                printf("  ");
            if(sab_c==1)
                printf(" * ");
        }
        printf("\n\n");
    }
    return 0;
}
