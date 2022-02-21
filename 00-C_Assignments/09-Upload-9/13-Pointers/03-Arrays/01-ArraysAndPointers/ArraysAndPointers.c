#include<stdio.h>

int main(int argc,char *argv[],char *envp[])
{
    // Variable Declarations
    int sab_iArray[] = {12,24,36,48,60,72,84,96,108,120};
    float sab_fArray[] = {9.8f,8.7f,7.6f,6.5f,5.4f};
    double sab_dArray[] = {1.222222,2.333333,3.4444444};
    char sab_cArray[] = {'A','S','T','R','O','M','E','D','I','C','O','M','P','\0'};

    // Code
    printf("\n\n");
    printf("Integer array elements and the addresses they occupy are as follows\n\n");
    printf("iArray[0] =  %d \t at address : %p\n",*(sab_iArray+0),(sab_iArray+0));
    printf("iArray[1] =  %d \t at address : %p\n",*(sab_iArray+1),(sab_iArray+1));
    printf("iArray[2] =  %d \t at address : %p\n",*(sab_iArray+2),(sab_iArray+2));
    printf("iArray[3] =  %d \t at address : %p\n",*(sab_iArray+3),(sab_iArray+3));
    printf("iArray[4] =  %d \t at address : %p\n",*(sab_iArray+4),(sab_iArray+4));
    printf("iArray[5] =  %d \t at address : %p\n",*(sab_iArray+5),(sab_iArray+5));
    printf("iArray[6] =  %d \t at address : %p\n",*(sab_iArray+6),(sab_iArray+6));
    printf("iArray[7] =  %d \t at address : %p\n",*(sab_iArray+7),(sab_iArray+7));
    printf("iArray[8] =  %d \t at address : %p\n",*(sab_iArray+8),(sab_iArray+8));
    printf("iArray[9] =  %d \t at address : %p\n",*(sab_iArray+9),(sab_iArray+9));

    printf("\n\n");
    printf("Floating array elements and the addresses they occupy are as follows\n\n");
    printf("fArray[0] =  %f \t at address : %p\n",*(sab_fArray+0),(sab_fArray+0));
    printf("fArray[1] =  %f \t at address : %p\n",*(sab_fArray+1),(sab_fArray+1));
    printf("fArray[2] =  %f \t at address : %p\n",*(sab_fArray+2),(sab_fArray+2));
    printf("fArray[3] =  %f \t at address : %p\n",*(sab_fArray+3),(sab_fArray+3));
    printf("fArray[4] =  %f \t at address : %p\n",*(sab_fArray+4),(sab_fArray+4));
    
    printf("\n\n");
    printf("Double array elements and the addresses they occupy are as follows\n\n");
    printf("dArray[0] =  %lf \t at address : %p\n",*(sab_dArray+0),(sab_dArray+0));
    printf("dArray[1] =  %lf \t at address : %p\n",*(sab_dArray+1),(sab_dArray+1));
    printf("dArray[2] =  %lf \t at address : %p\n",*(sab_dArray+2),(sab_dArray+2));

    printf("\n\n");
    printf("Character array elements and the addresses they occupy are as follows\n\n");
    printf("cArray[0] =  %c \t at address : %p\n",*(sab_cArray+0),(sab_cArray+0));
    printf("cArray[1] =  %c \t at address : %p\n",*(sab_cArray+1),(sab_cArray+1));
    printf("cArray[2] =  %c \t at address : %p\n",*(sab_cArray+2),(sab_cArray+2));
    printf("cArray[3] =  %c \t at address : %p\n",*(sab_cArray+3),(sab_cArray+3));
    printf("cArray[4] =  %c \t at address : %p\n",*(sab_cArray+4),(sab_cArray+4));
    printf("cArray[5] =  %c \t at address : %p\n",*(sab_cArray+5),(sab_cArray+5));
    printf("cArray[6] =  %c \t at address : %p\n",*(sab_cArray+6),(sab_cArray+6));
    printf("cArray[7] =  %c \t at address : %p\n",*(sab_cArray+7),(sab_cArray+7));
    printf("cArray[8] =  %c \t at address : %p\n",*(sab_cArray+8),(sab_cArray+8));
    printf("cArray[9] =  %c \t at address : %p\n",*(sab_cArray+9),(sab_cArray+9));
    printf("cArray[10] =  %c \t at address : %p\n",*(sab_cArray+10),(sab_cArray+10));
    printf("cArray[11] =  %c \t at address : %p\n",*(sab_cArray+11),(sab_cArray+11));
    printf("cArray[12] =  %c \t at address : %p\n",*(sab_cArray+12),(sab_cArray+12));
    printf("cArray[13] =  %c \t at address : %p\n",*(sab_cArray+13),(sab_cArray+13));
    return 0;
}
