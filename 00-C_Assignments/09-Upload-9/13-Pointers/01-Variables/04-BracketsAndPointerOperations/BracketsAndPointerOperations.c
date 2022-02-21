#include <stdio.h>

int main(int argc,char *argv[],char *envp[])
{
    // Variable declarations
    int sab_num;
    int *sab_ptr = NULL;
    // Code
    sab_num = 5;
    sab_ptr = &sab_num;
    printf("\n\n");
    printf(" num    = %d\n", sab_num);
    printf(" &num   = %p\n", &sab_num);
    printf(" *(&num)= %d\n", *(&sab_num));
    printf(" ptr    = %p\n", sab_ptr);
    printf(" *ptr   = %d\n", *sab_ptr);
    printf("\n\n");
    printf("Answer of (ptr + 10)  = %p\n",  (sab_ptr+10));
    printf("Answer of *(ptr + 10) = %d\n", *(sab_ptr+10));
    printf("Answer of (*ptr + 10) = %d\n\n",(*sab_ptr+10));
    ++*sab_ptr;
    printf("Answer of ++*ptr : %d\n",*sab_ptr);
    *sab_ptr++;
    printf("Answer of *ptr++ : %d\n",*sab_ptr);
    sab_ptr = &sab_num;
    (*sab_ptr)++;
    printf("Answer of (*ptr)++ : %d\n\n",*sab_ptr);
    return 0;
}
