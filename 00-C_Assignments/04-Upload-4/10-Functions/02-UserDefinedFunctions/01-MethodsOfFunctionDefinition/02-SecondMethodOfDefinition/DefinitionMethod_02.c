#include<stdio.h>
int main(int argc,char *argv[],char *envp[])
{
    //Function signature
    int sab_MyAddition(void);

    int sab_result;
    //code
    sab_result=sab_MyAddition();
    printf("\n\n");
    printf("Sum = %d\n\n",sab_result);
    return 0;

}

int sab_MyAddition(void)
{
    int sab_a,sab_b,sab_sum;

    //code
    printf("\n\n");
    printf("Enter integer value for 'A' : ");
    scanf("%d",&sab_a);

    printf("\n\n");
    printf("Enter integer value for 'B' : ");
    scanf("%d",&sab_b);

    sab_sum=sab_a+sab_b;

    return sab_sum;
}