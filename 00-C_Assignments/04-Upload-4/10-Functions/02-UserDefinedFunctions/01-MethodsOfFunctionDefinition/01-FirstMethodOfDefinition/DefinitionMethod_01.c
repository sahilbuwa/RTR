#include<stdio.h>
int main(int argc,char *argv[],char *envp[])
{
    //Function signature
    void sab_MyAddition(void);

    //code
    sab_MyAddition();
    return 0;

}

void sab_MyAddition(void)
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

    printf("\n\n");
    printf("Sum of %d and %d is %d\n",sab_a,sab_b,sab_sum);
}