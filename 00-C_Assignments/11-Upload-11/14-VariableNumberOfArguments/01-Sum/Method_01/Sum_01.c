#include <stdio.h>
#include <stdarg.h>

int main(int argc,char *argv[],char *envp[])
{
    // Function prototypes
    int sab_CalculateSum(int, ...);

    // Variable declarations
    int sab_answer;

    // Code

    printf("\n\n");
    sab_answer=sab_CalculateSum(5,10,20,30,40,50);

    printf("Answer= %d\n\n",sab_answer);
    sab_answer=sab_CalculateSum(10,1,1,1,1,1,1,1,1,1,1);

    printf("Answer= %d\n\n",sab_answer);
    sab_answer=sab_CalculateSum(0);

    printf("Answer= %d\n\n",sab_answer);
    
    return 0;
}

int sab_CalculateSum(int sab_num, ...)
{
    // Variable declarations
    int sab_sum_total = 0;
    int sab_n;
    va_list sab_numbers_list;

    // Code
    va_start(sab_numbers_list,sab_num);
    while(sab_num)
    {
        sab_n=va_arg(sab_numbers_list,int);
        sab_sum_total=sab_sum_total+sab_n;
        sab_num--;
    }
    va_end(sab_numbers_list);
    return sab_sum_total;
}
