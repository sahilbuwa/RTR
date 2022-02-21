#include<stdio.h>

int sab_global_count=0;

int main(void)
{
    //Function Prototypes
    void sab_change_count_one(void);
    void sab_change_count_two(void);
    void sab_change_count_three(void);
    //code
    printf("\n");

    printf("Main(): value of global_count = %d\n",sab_global_count);

    sab_change_count_one();
    sab_change_count_two();
    sab_change_count_three();

    printf("\n");
    return 0;
}

void sab_change_count_one(void)
{
    //code
    sab_global_count = 100;
    printf("Change count one (): Value of global count = %d\n",sab_global_count);
}

void sab_change_count_two(void)
{
    //code
    sab_global_count = sab_global_count + 1;
    printf("Change_count_two() ; value of global count = %d\n",sab_global_count);

}

void sab_change_count_three(void)
{
    //code
    sab_global_count = sab_global_count + 10;
    printf("Change_count_three() ; value of global count = %d\n",sab_global_count);

}
