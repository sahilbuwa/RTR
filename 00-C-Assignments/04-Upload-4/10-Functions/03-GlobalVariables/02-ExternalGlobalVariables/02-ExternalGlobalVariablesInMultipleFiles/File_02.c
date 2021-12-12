#include <stdio.h>
void sab_change_count_two(void)
{
    //code
    extern int sab_global_count;
    sab_global_count = sab_global_count + 1;
    printf("change_count_two() : Value Of global_count in File_02 = %d\n",sab_global_count);
}