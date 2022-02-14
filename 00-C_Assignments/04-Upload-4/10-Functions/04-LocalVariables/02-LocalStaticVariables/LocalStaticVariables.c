#include<stdio.h>

int main(void)
{
    int sab_a=5;

    void sab_change_count(void);

    //code
    printf("\n");
    printf("A = %d\n\n",sab_a);

    sab_change_count();
    sab_change_count();
    sab_change_count();

    return 0;

}

void sab_change_count(void)
{
    static int sab_local_count = 0;

    //code
    sab_local_count = sab_local_count + 1;
    printf("Local Count = %d\n",sab_local_count);

}
