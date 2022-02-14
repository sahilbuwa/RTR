#include<stdio.h>

int main(int argc, char *argv[],char *envp[])
{
    //Function Declarations
    int sab_MyAddition(int,int);

    //Variable Declarations
    int sab_r;
    int sab_num1,sab_num2,sab_num3,sab_num4;

    //code
    sab_num1=10;
    sab_num2=20;
    sab_num3=30;
    sab_num4=40;

    sab_r=sab_MyAddition(sab_MyAddition(sab_num1,sab_num2),sab_MyAddition(sab_num3,sab_num4));

    printf("\n\n");
    printf("%d + %d + %d + %d = %d\n",sab_num1,sab_num2,sab_num3,sab_num4,sab_r);
    printf("\n\n");

    return 0;
}

int sab_MyAddition(int sab_a,int sab_b)
{
    int sab_sum;
    sab_sum =sab_a + sab_b;
    return sab_sum;
}