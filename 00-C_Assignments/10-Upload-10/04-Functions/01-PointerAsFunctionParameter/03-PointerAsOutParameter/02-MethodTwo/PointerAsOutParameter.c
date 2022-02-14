#include<stdio.h>
#include<stdlib.h>

int main(int argc,char* argv[],char *envp[])
{
    // Function Declarations
    void sab_MathematicalOperations(int,int,int *,int *,int *,int *,int *);

    // Variable Declaration
    int sab_a,sab_b,*sab_answer_sum=NULL,*sab_answer_difference=NULL,*sab_answer_product=NULL,*sab_answer_quotient=NULL,*sab_answer_remainder=NULL;

    // Code
    printf("\n\n");
    printf("Enter value of 'A': ");
    scanf("%d",&sab_a);

    printf("\n\n");
    printf("Enter value of 'B': ");
    scanf("%d",&sab_b);

    sab_answer_sum=(int *)malloc(1*sizeof(int));
    if(sab_answer_sum==NULL)
    {
        printf("Could not allocate memory for sum...Exiting now\n\n");
        exit(0);
    }
    sab_answer_difference=(int *)malloc(1*sizeof(int));
    if(sab_answer_difference==NULL)
    {
        printf("Could not allocate memory for difference...Exiting now\n\n");
        exit(0);
    }
    sab_answer_product=(int *)malloc(1*sizeof(int));
    if(sab_answer_product==NULL)
    {
        printf("Could not allocate memory for product...Exiting now\n\n");
        exit(0);
    }
    sab_answer_quotient=(int *)malloc(1*sizeof(int));
    if(sab_answer_quotient==NULL)
    {
        printf("Could not allocate memory for quotient...Exiting now\n\n");
        exit(0);
    }
    sab_answer_remainder=(int *)malloc(1*sizeof(int));
    if(sab_answer_remainder==NULL)
    {
        printf("Could not allocate memory for remainder...Exiting now\n\n");
        exit(0);
    }

    sab_MathematicalOperations(sab_a,sab_b,sab_answer_sum,sab_answer_difference,sab_answer_product,sab_answer_quotient,sab_answer_remainder);

    printf("\n\n");
    printf("*** Results *** : \n\n");
    printf("Sum = %d\n\n",*sab_answer_sum);
    printf("Difference = %d\n\n",*sab_answer_difference);
    printf("Product = %d\n\n",*sab_answer_product);
    printf("Quotient = %d\n\n",*sab_answer_quotient);
    printf("Remainder = %d\n\n",*sab_answer_remainder);

    if(sab_answer_remainder)
    {
        free(sab_answer_remainder);
        sab_answer_remainder=NULL;
        printf("Memory allocated for remainder freed!!!\n\n");
    }
    if(sab_answer_quotient)
    {
        free(sab_answer_quotient);
        sab_answer_quotient=NULL;
        printf("Memory allocated for quotient freed!!!\n\n");
    }
    if(sab_answer_product)
    {
        free(sab_answer_product);
        sab_answer_product=NULL;
        printf("Memory allocated for product freed!!!\n\n");
    }
    if(sab_answer_difference)
    {
        free(sab_answer_difference);
        sab_answer_difference=NULL;
        printf("Memory allocated for difference freed!!!\n\n");
    }
    if(sab_answer_sum)
    {
        free(sab_answer_sum);
        sab_answer_sum=NULL;
        printf("Memory allocated for sum freed!!!\n\n");
    }
    
    return 0;
}

void sab_MathematicalOperations(int sab_x,int sab_y,int *sab_sum,int *sab_difference,int *sab_product,int *sab_quotient,int *sab_remainder)
{
    // Code
    *sab_sum = sab_x+sab_y;
    *sab_difference = sab_x-sab_y;
    *sab_product=sab_x*sab_y;
    *sab_quotient=sab_x/sab_y;
    *sab_remainder=sab_x%sab_y;
}
