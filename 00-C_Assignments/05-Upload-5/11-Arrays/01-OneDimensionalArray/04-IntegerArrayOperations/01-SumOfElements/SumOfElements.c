#include<stdio.h>

#define NUM_ELEMENTS 10 
int main(void)
{
    //variable declarations
    int sab_iArray[NUM_ELEMENTS];
    int sab_i,sab_sum=0,sab_num;

    //Code
    printf("\n\n");
    printf("Enter Integer elements for array : \n\n");
    for(sab_i=0;sab_i< NUM_ELEMENTS;sab_i++)
    {
        scanf("%d",&sab_num);
        sab_iArray[sab_i]=sab_num;
    }
    for(sab_i=0;sab_i<NUM_ELEMENTS;sab_i++)
    {
        sab_sum=sab_sum + sab_iArray[sab_i];
    }
    printf("\n\n");
    printf("Sum of all elements of array = %d\n\n",sab_sum);

    return 0;
}
