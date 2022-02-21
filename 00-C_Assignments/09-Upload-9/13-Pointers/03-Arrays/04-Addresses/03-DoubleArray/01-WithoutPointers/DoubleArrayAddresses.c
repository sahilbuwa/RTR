#include <stdio.h>

int main(int argc,char *argv[],char *envp[])
{
    // Variable declarations
    double sab_dArray[10];
    int i;
    // Code
    for (i = 0; i < 10; i++)
        sab_dArray[i] = (float)(i + 1)*1.33333f;

    printf("\n\n");
    printf("Elements of the double array: \n\n");

    for (i = 0; i < 10; i++)
        printf("dArray[%d] = %lf\n", i,sab_dArray[i]);

    printf("\n\n");
    printf("Elements of the double array: \n\n");

    for (i = 0; i < 10; i++)
        printf("dArray[%d] = %lf \t \t address = %p\n", i,sab_dArray[i],&sab_dArray[i]);
    
    printf("\n\n");
    return 0;
}
