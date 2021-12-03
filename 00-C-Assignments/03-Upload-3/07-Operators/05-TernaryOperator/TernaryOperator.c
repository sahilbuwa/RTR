#include<stdio.h>

int main(void)
{
    //Variable declarations
    int sab_a,sab_b;
    int sab_p,sab_q;
    char sab_ch_result_01,sab_ch_result_02;
    int sab_i_result_01,sab_i_result_02;

    //code
    sab_a=7;
    sab_b-5;
    sab_ch_result_01=(sab_a>sab_b) ? 'A' : 'B';
    sab_i_result_01= (sab_a>sab_b) ? sab_a:sab_b;
    printf("Ternary Operator Answer 1 ----- %c and %d.\n\n",sab_ch_result_01,sab_i_result_01);

    sab_p=30;
    sab_q=30;
    sab_ch_result_02=(sab_p!=sab_q) ? 'P' : 'Q';
    sab_i_result_02 = (sab_p != sab_q ) ? sab_p : sab_q;
    printf("Ternary Operator answer 2 ----- %c and %d.\n\n",sab_ch_result_02,sab_i_result_02);

    printf("\n\n");
    return 0;

}
