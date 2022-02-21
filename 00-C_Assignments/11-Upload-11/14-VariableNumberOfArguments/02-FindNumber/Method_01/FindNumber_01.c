#include <stdio.h>
#include <stdarg.h>

#define NUM_TO_BE_FOUND 3
#define NUM_ELEMENTS 10

int main(int argc,char *argv,char *envp[])
{
    // Function Prototypes
    void sab_FindNumber(int, int, ...);

    // Code
    printf("\n\n");
    sab_FindNumber(NUM_TO_BE_FOUND,NUM_ELEMENTS,3,5,9,2,3,6,9,3,1,3);

    return 0;
}

void sab_FindNumber(int sab_num_to_be_found,int sab_num, ...)
{
    // Variable Declarations
    int sab_count=0,sab_n;
    va_list sab_numbers_list;

    // Code

    va_start(sab_numbers_list,sab_num);
    while(sab_num)
    {
        sab_n=va_arg(sab_numbers_list,int);
        if(sab_n==sab_num_to_be_found)
            sab_count++;
        
        sab_num--;
    }

    if(sab_count==0)
        printf("Number %d could not be found!\n\n",sab_num_to_be_found);
    
    else
        printf("Number %d found %d times!!\n\n",sab_num_to_be_found,sab_count);
    
    va_end(sab_numbers_list);
}
