#include <stdio.h>

int sab_global_count = 0;

int main(void)
{
    //function prototypes
    void sab_change_count(void);
    void sab_change_count_one(void); 
    void sab_change_count_two(void); 
    //code
    printf("\n\n");
    sab_change_count();
    sab_change_count_one();
    sab_change_count_two(); 
    return(0);
}
void sab_change_count(void)
{
    //code
    sab_global_count = sab_global_count + 1;
    printf("Global Count = %d\n", sab_global_count);
}