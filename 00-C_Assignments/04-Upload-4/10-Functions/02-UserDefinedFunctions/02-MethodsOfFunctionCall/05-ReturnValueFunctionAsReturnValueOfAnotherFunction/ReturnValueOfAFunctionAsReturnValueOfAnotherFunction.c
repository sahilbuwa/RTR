#include<stdio.h>

int main(int argc , char * argv[],char *envp[])
{
    int sab_MyAddition(int,int);

    int sab_r;
    int sab_num1,sab_num2;

    //code
    sab_num1=10;
    sab_num2=20;

    printf("\n\n");
    printf("%d + %d = %d\n",sab_num1,sab_num2,sab_MyAddition(sab_num1,sab_num2));
    printf("\n\n");

    return 0;
}

int sab_MyAddition(int sab_a,int sab_b)
{
    int sab_add(int,int);

    return sab_add(sab_a,sab_b);
}

int sab_add(int sab_x,int sab_y)
{
    return sab_x+sab_y;
}
