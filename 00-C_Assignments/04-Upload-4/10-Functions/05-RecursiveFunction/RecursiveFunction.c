#include<stdio.h>

int main(int argc,char *argv[],char *envp[])
{
    //Variable Declarations
    unsigned int sab_num;

    //Function Prototype
    void sab_recursive(unsigned int);

    //code
    printf("\n\n");
    printf("Enter Any Number : \n\n");
    scanf("%u", &sab_num);

    printf("\n\n");
    printf("Output Of Recursive Function : \n\n");

    sab_recursive(sab_num);
    printf("\n\n");
    return(0);
}
void sab_recursive(unsigned int sab_n)
{
    // code
    printf("n = %d\n", sab_n);
    if(sab_n > 0)
    {
        sab_recursive(sab_n - 1);
    }
}