#include <stdio.h>

int main(void)
{
    //variable declartions
    int sab_i,sab_j;
    char sab_ch_01,sab_ch_02;

    int sab_a,sab_result_int;
    float sab_f, sab_result_float;

    int sab_i_explicit;
    float sab_f_explicit;

    //code
    printf("\n\n");

    sab_i=70;
    sab_ch_01= sab_i;
    printf("I = %d \n  ", sab_i);
    printf("Character 1 (after ch_01 = i) = %c\n\n",sab_ch_01);

    sab_ch_02 = 'Q';
    sab_j = sab_ch_02;
    printf("Character 2 = %c\n", sab_ch_02);
    printf("J (after j = ch_02) = %d\n\n",sab_j);

    //Implicit conversion of int to float
    sab_a=5;
    sab_f = 7.8f;
    sab_result_float = sab_a + sab_f;
    printf("Integer a = %d and floating point number %f added gives floating point sum = %f\n\n",sab_a,sab_f,sab_result_float);

    sab_result_int = sab_a + sab_f;
    printf("Integer a = %d and floating point number %f added gives integer sum %d\n\n", sab_a,sab_f,sab_result_int);

    //Explicit type casting using cast operator
    sab_f_explicit = 30.121995f;
    sab_i_explicit = (int) sab_f_explicit;
    printf("Floating point number which will be type casted explicitly = %f\n", sab_f_explicit);
    printf("Resultant integer after explicit type casting of %f = %d\n\n",sab_f_explicit,sab_i_explicit);

    return 0;

}
