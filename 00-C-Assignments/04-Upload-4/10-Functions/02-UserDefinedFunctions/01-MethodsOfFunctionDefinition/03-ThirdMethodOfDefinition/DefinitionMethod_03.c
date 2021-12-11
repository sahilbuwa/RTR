#include<stdio.h>
int main(int argc,char *argv[],char *envp[])
{
    //Function signature
    void sab_MyAddition(int,int);

    int sab_result,sab_a,sab_b;
    //code
    printf("\n\n");
    printf("Enter integer value for 'A' : ");
    scanf("%d",&sab_a);

    printf("\n\n");
    printf("Enter integer value for 'B' : ");
    scanf("%d",&sab_b);

    sab_MyAddition(sab_a,sab_b);
    
    return 0;

}

void sab_MyAddition(int sab_a,int sab_b)
{
    int sab_sum;

    //code
    sab_sum=sab_a+sab_b;
    printf("\n\n");
    printf("Sum = %d\n\n",sab_sum);

}