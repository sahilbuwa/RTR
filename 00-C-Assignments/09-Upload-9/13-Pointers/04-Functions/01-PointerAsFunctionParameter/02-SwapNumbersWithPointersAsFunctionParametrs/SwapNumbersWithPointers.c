#include<stdio.h>

int main(int argc,char* argv[],char* envp[])
{
    // Function Declarations
    void sab_SwapNumbers(int*,int*);

    // Variable Declarations
    int sab_a,sab_b;

    // Code
    printf("\n\n");
    printf("Enter value for 'A' : ");
    scanf("%d",&sab_a);

    printf("\n\n");
    printf("Enter value for 'B' : ");
    scanf("%d",&sab_b);

    printf("\n\n");
    printf("********Before Swapping******\n\n");
    printf("Value of 'A' =%d\n\n",sab_a);
    printf("Value of 'B' =%d\n\n",sab_b);

    sab_SwapNumbers(&sab_a,&sab_b);

    printf("\n\n");
    printf("*****After Swapping******\n\n");
    printf("Value of 'A' =%d\n\n",sab_a);
    printf("Value of 'B' =%d\n\n",sab_b);
    
    return 0;
}

void sab_SwapNumbers(int *sab_x,int *sab_y)
{
    // Variable Declarations
    int sab_temp;

    // Code
     printf("\n\n");
    printf("********Before Swapping******\n\n");
    printf("Value of 'X' =%d\n\n",*sab_x);
    printf("Value of 'Y' =%d\n\n",*sab_y);

    sab_temp = *sab_x;
    *sab_x = *sab_y;
    *sab_y = sab_temp;

    printf("\n\n");
    printf("*****After Swapping******\n\n");
    printf("Value of 'X' =%d\n\n",*sab_x);
    printf("Value of 'Y' =%d\n\n",*sab_y);

}
