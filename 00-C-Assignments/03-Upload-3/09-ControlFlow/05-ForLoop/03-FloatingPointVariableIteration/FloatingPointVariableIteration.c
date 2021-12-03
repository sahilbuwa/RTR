#include<stdio.h>
int main(void)
{
    //Variable Declarations
    float sab_f;
    float sab_f_num = 1.7f;

    //code
    printf("\n\n");

    printf("Printing Numbers %f to %f : \n\n",sab_f_num,(sab_f_num * 10.0f));

    for(sab_f= sab_f_num; sab_f <= (sab_f_num * 10.0f); sab_f = sab_f +sab_f_num )
    {
        printf("\t%f\n",sab_f);
    }

    printf("\n\n");
    return 0;
}
