#include<stdio.h>

int main(void)
{
    //variable declarations
    int sab_a;
    int sab_b;
    int sab_result;

    //code
    printf("\n\n");
    printf("Enter One Integer : ");
    scanf("%d",&sab_a);

    printf("\n\n");
    printf("Enter Another Integer : ");
    scanf("%d",&sab_b);

    printf("\n\n");
    printf("If Answer=0, It Is 'FALSE'.\n");
    printf("If Answer=1, It is 'TRUE'.\n\n");

    sab_result=(sab_a<sab_b);
    printf("(a < b) A = %d Is Less Than B =%d       \t Answer = %d\n",sab_a,sab_b,sab_result);

    sab_result=(sab_a>sab_b);
    printf("(a > b) A = %d Is Greater Than B =%d       \t Answer = %d\n",sab_a,sab_b,sab_result);
    
    sab_result=(sab_a<=sab_b);
    printf("(a <= b) A = %d Is Less Than Or Equal To B =%d       \t Answer = %d\n",sab_a,sab_b,sab_result);
    
    sab_result=(sab_a>=sab_b);
    printf("(a >= b) A = %d Is Greater Than or Equal to B =%d       \t Answer = %d\n",sab_a,sab_b,sab_result);
    
    sab_result=(sab_a==sab_b);
    printf("(a == b) A = %d Is Equal To B =%d       \t Answer = %d\n",sab_a,sab_b,sab_result);
    
    sab_result=(sab_a!=sab_b);
    printf("(a != b) A = %d Is Not Equal To B =%d       \t Answer = %d\n",sab_a,sab_b,sab_result);
    
    return 0;
}
