#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
    // Variable Declarations
    int sab_num, *sab_ptr = NULL , *sab_copy_ptr = NULL;

    // Code
    sab_num = 5;
    sab_ptr = &sab_num;

    printf("\n\n");
    printf("+++++++++Before copy_ptr = ptr ++++++++\n\n");
    printf("    Num         = %d\n",sab_num);
    printf("    &num        = %p\n",&sab_num);
    printf("    *(&num)     = %d\n",*(&sab_num));
    printf("    ptr         = %p\n",sab_ptr);
    printf("    *ptr        = %d\n",*sab_ptr);


    sab_copy_ptr = sab_ptr;

    printf("\n\n");
    printf("+++++++++Before copy_ptr = ptr ++++++++\n\n");
    printf("    Num         = %d\n",sab_num);
    printf("    &num        = %p\n",&sab_num);
    printf("    *(&num)     = %d\n",*(&sab_num));
    printf("    ptr         = %p\n",sab_ptr);
    printf("    *ptr        = %d\n",*sab_ptr);
    printf("    copy ptr    = %p\n",sab_copy_ptr);
    printf("   *copy ptr    = %d\n",*sab_copy_ptr);
    return 0;

}
