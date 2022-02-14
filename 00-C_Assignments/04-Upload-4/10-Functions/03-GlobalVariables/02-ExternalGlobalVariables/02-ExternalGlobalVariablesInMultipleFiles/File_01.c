#include <stdio.h>
extern int sab_global_count;
void sab_change_count_one(void)
{
    //code
    sab_global_count = sab_global_count + 1;
    printf("change_count_one() : Value Of global_count in File_01 = %d\n",sab_global_count);
}