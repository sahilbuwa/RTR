#include<stdio.h>

int main(int argc,char* argv[],char* envp[])
{
    void sab_MathematicalOperations(int,int,int*,int*,int*,int*,int*);

    // Variable Declarations
    int sab_a,sab_b,sab_answer_sum,sab_answer_difference,sab_answer_product,sab_answer_quotient,sab_answer_remainder;

    // Code
    printf("\n\n");
    printf("Ener value of 'A' : ");
    scanf("%d",&sab_a);

    printf("\n\n");
    printf("Enter value of 'B' : ");
    scanf("%d",&sab_b);

    sab_MathematicalOperations(sab_a,sab_b,&sab_answer_sum,&sab_answer_difference,&sab_answer_product,&sab_answer_quotient,&sab_answer_remainder);

    printf("\n\n");
    printf("******Results******** : \n\n");
    printf("Sum = %d\n\n",sab_answer_sum);
    printf("Difference = %d\n\n",sab_answer_difference);
    printf("Product =%d\n\n",sab_answer_product);
    printf("Quotient = %d\n\n",sab_answer_quotient);
    printf("Remainder =%d\n\n",sab_answer_remainder);

    return 0;
}

void sab_MathematicalOperations(int sab_x,int sab_y,int *sab_sum,int *sab_difference,int *sab_product,int *sab_quotient,int *sab_remainder)
{
    // Code
    *sab_sum = sab_x+sab_y;
    *sab_difference = sab_x - sab_y;
    *sab_product = sab_x*sab_y;
    *sab_quotient = sab_x/sab_y;
    *sab_remainder = sab_x%sab_y;
}
