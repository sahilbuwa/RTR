#include<stdio.h>

int main(int argc,char *argv[],char *envp[])
{
    // Variable Declarations
    int sab_num=5;
    int* const sab_ptr = &sab_num;

    // Code
    printf("\n");
    printf("Current value of 'num' = %d\n",sab_num);
    printf("Current 'ptr' (address of 'num') = %p\n",sab_ptr);

    sab_num++;
    printf("\n\n");
    printf("After num++, value of 'num'=%d\n",sab_num);
    (*sab_ptr)++;
    //sab_ptr++;

    printf("\n\n");
    printf("After ptr++,value of 'ptr' = %p\n",sab_ptr);
    printf("Value at this new 'ptr' =%d\n",*sab_ptr);
    printf("\n\n");
    return 0;
}
