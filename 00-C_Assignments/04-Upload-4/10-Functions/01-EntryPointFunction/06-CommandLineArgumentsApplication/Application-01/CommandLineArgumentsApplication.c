#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>

int main(int argc, char *argv[],char *envp[])
{
    //Variable Declarations
    int sab_num,sab_sum=0;
    //code
    if(argc==1)
    {
        printf("\n\n");
        printf("No Numbers Given For Addition !!! Exitting now ...\n\n");
        printf("Usage : CommandLineArgumentsApplication <first number> <second number> ...\n\n");
        exit(0);
    }
    printf("\n\n");
    printf("Sum of all integer command line arguments is : \n\n");
    for(int i=1;i<argc;i++)
    {
        sab_num=atoi(argv[i]);
        sab_sum=sab_sum+sab_num;

    }
    printf("sum = %d \n\n",sab_sum);
    return 0;
}