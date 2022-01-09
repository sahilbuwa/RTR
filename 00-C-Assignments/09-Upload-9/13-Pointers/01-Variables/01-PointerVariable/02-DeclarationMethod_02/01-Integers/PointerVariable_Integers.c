#include<stdio.h>

int main(int argc,char *argv[],char *envp[])
{
    // Variable Declarations
    int sab_num;
    int* sab_ptr = NULL;

    // Code
    sab_num = 10;
    printf("\n\n");

    printf("***********Before ptr = &num *********\n\n");
    printf("Value Of num            =%d\n\n ", sab_num);
    printf("Address of num          =%p\n\n",&sab_num);
    printf("Value at address of num =%d\n\n",*(&sab_num));

    sab_ptr = &sab_num;

    printf("\n\n");

    printf("**********After ptr = &num *******\n\n");
    printf("Value Of num            =%d\n\n ", sab_num);
    printf("Address of num          =%p\n\n",sab_ptr);
    printf("Value at address of num =%d\n\n",*sab_ptr);

    return 0;
}
