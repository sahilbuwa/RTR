#include <stdio.h>
#define NUM_ELEMENTS 10

int main(void)
{
    //variable declarations
    int sab_iArray[NUM_ELEMENTS];
    int i, sab_num, sab_sum = 0;
    //code
    printf("\n\n");
    
    printf("Enter Integer Elememts For Array : \n\n");
    
    for (i = 0; i < NUM_ELEMENTS; i++)
    {
        scanf("%d", &sab_num);
        sab_iArray[i] = sab_num;
    }
    
    printf("\n\n");
    printf("Even Numbers Amongst The Array Elements Are : \n\n");

    for (i = 0; i < NUM_ELEMENTS; i++)
    {
        if ((sab_iArray[i] % 2) == 0)
        printf("%d\n", sab_iArray[i]);
    }
    
    printf("\n\n");
    printf("Odd Numbers Amongst The Array Elements Are : \n\n");

    for (i = 0; i < NUM_ELEMENTS; i++)
    {
        if ((sab_iArray[i] % 2) != 0)
        printf("%d\n", sab_iArray[i]);
    }
    return 0;
}
