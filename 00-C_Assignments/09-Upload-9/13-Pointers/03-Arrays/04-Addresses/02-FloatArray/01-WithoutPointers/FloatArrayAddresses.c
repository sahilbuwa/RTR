#include <stdio.h>

int main(int argc,char *argv[],char *envp[])
{
    // Variable declarations
    float sab_fArray[10];
    int i;
    // Code
    for (i = 0; i < 10; i++)
        sab_fArray[i] = (float)(i + 1)*1.5f;

    printf("\n\n");
    printf("Elements of the float array: \n\n");

    for (i = 0; i < 10; i++)
        printf("fArray[%d] = %f\n", i,sab_fArray[i]);

    printf("\n\n");
    printf("Elements of the float array: \n\n");

    for (i = 0; i < 10; i++)
        printf("fArray[%d] = %f \t \t address = %p\n", i,sab_fArray[i],&sab_fArray[i]);
    
    printf("\n\n");
    return 0;
}
