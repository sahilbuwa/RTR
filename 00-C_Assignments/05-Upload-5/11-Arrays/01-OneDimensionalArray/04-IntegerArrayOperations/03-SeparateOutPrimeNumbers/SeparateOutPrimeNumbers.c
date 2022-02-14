#include <stdio.h>

#define NUM_ELEMENTS 10

int main(void)
{
    //variable declarations
    int sab_iArray[NUM_ELEMENTS];
    int i, sab_num, j, sab_count = 0;
    //code
    printf("\n\n");
    
    printf("Enter Integer Elememts For Array : \n\n");
    for (i = 0; i < NUM_ELEMENTS; i++)
    {
        scanf("%d", &sab_num);

        if (sab_num < 0)
            sab_num = -1 * sab_num;

        sab_iArray[i] = sab_num;
    }
    
    printf("\n\n");
    printf("Array Elements Are : \n\n");
    for (i = 0; i < NUM_ELEMENTS; i++)
        printf("%d\n", sab_iArray[i]);
   
    printf("Prime Numbers Amongst The Array Elements Are : \n\n");
    for (i = 0; i < NUM_ELEMENTS; i++)
    {
        for (j = 1; j <= sab_iArray[i]; j++)
        {
            if ((sab_iArray[i] % j) == 0)
                sab_count++;
        }
        
        if (sab_count == 2)
            printf("%d\n", sab_iArray[i]);

        sab_count = 0; 
    }
    return 0;
}
