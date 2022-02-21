#include<stdio.h>

int main(void)
{
    void sab_change_count(void);
    //variable declarations
    extern int sab_global_count;

    printf("\n");
    printf("Value Of global_count before change_count() = %d\n", sab_global_count);
    sab_change_count();
    printf("Value Of global_count after change_count() = %d\n", sab_global_count);
    printf("\n");
    return(0);
}

int sab_global_count = 0;

void sab_change_count(void)
{
    //code
    sab_global_count = 5;
    printf("value of global count in change count () = %d\n",sab_global_count);

}
