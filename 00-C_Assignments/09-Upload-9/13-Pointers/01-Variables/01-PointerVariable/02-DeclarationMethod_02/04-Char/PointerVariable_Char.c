#include<stdio.h>

int main(int argc,char *argv[],char *envp[])
{
    // Variable Declarations
    char sab_ch;
    char* sab_ptr = NULL;

    // Code
    sab_ch = 'S';
    printf("\n\n");

    printf("***********Before ptr = &num *********\n\n");
    printf("Value Of num            =%c\n\n ", sab_ch);
    printf("Address of num          =%p\n\n",&sab_ch);
    printf("Value at address of num =%c\n\n",*(&sab_ch));

    sab_ptr = &sab_ch;

    printf("\n\n");

    printf("**********After ptr = &num *******\n\n");
    printf("Value Of num            =%c\n\n ", sab_ch);
    printf("Address of num          =%p\n\n",sab_ptr);
    printf("Value at address of num =%c\n\n",*sab_ptr);

    return 0;
}
