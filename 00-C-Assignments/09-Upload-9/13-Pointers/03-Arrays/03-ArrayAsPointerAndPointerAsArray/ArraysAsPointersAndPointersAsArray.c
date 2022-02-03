#include <stdio.h>

int main(int argc,char *argv[],char *envp[])
{
    // variable declarations
    int sab_iArray[] = {10,20,30,40,50,60,70,80,90,100};
    int *sab_ptr_iArray = NULL;
    // code
    printf("\n\n");
    printf("**Using array name as a pointer i.e: value Of xth element of iArray : *(iArray + x) and address of xth element of iArray : (iArray + x)**\n\n");
    printf("Integer array elements and their addresses : \n\n");
    printf("*(iArray + 0) = %d \t at address (iArray + 0) : %p\n",*(sab_iArray + 0),(sab_iArray + 0));
    printf("*(iArray + 1) = %d \t at address (iArray + 1) : %p\n",*(sab_iArray + 1),(sab_iArray + 1));
    printf("*(iArray + 2) = %d \t at address (iArray + 2) : %p\n",*(sab_iArray + 2),(sab_iArray + 2));
    printf("*(iArray + 3) = %d \t at address (iArray + 3) : %p\n",*(sab_iArray + 3),(sab_iArray + 3));
    printf("*(iArray + 4) = %d \t at address (iArray + 4) : %p\n",*(sab_iArray + 4),(sab_iArray + 4));
    printf("*(iArray + 5) = %d \t at address (iArray + 5) : %p\n",*(sab_iArray + 5),(sab_iArray + 5));
    printf("*(iArray + 6) = %d \t at address (iArray + 6) : %p\n",*(sab_iArray + 6),(sab_iArray + 6));
    printf("*(iArray + 7) = %d \t at address (iArray + 7) : %p\n",*(sab_iArray + 7),(sab_iArray + 7));
    printf("*(iArray + 8) = %d \t at address (iArray + 8) : %p\n",*(sab_iArray + 8),(sab_iArray + 8));
    printf("*(iArray + 9) = %d \t at address (iArray + 9) : %p\n",*(sab_iArray + 9),(sab_iArray + 9));

    sab_ptr_iArray = sab_iArray;

    printf("\n\n");
    printf("Using pointer as array name");
    printf("Integer Array Elements And Their Addresses : \n\n");
    printf("ptr_iArray[0] = %d \t at address &ptr_iArray[0] : %p\n",sab_ptr_iArray[0], &sab_ptr_iArray[0]);
    printf("ptr_iArray[1] = %d \t at address &ptr_iArray[1] : %p\n",sab_ptr_iArray[1], &sab_ptr_iArray[1]);
    printf("ptr_iArray[2] = %d \t at address &ptr_iArray[2] : %p\n",sab_ptr_iArray[2], &sab_ptr_iArray[2]);
    printf("ptr_iArray[3] = %d \t at address &ptr_iArray[3] : %p\n",sab_ptr_iArray[3], &sab_ptr_iArray[3]);
    printf("ptr_iArray[4] = %d \t at address &ptr_iArray[4] : %p\n",sab_ptr_iArray[4], &sab_ptr_iArray[4]);
    printf("ptr_iArray[5] = %d \t at address &ptr_iArray[5] : %p\n",sab_ptr_iArray[5], &sab_ptr_iArray[5]);
    printf("ptr_iArray[6] = %d \t at address &ptr_iArray[6] : %p\n",sab_ptr_iArray[6], &sab_ptr_iArray[6]);
    printf("ptr_iArray[7] = %d \t at address &ptr_iArray[7] : %p\n",sab_ptr_iArray[7], &sab_ptr_iArray[7]);
    printf("ptr_iArray[8] = %d \t at address &ptr_iArray[8] : %p\n",sab_ptr_iArray[8], &sab_ptr_iArray[8]);
    printf("ptr_iArray[9] = %d \t at address &ptr_iArray[9] : %p\n",sab_ptr_iArray[9], &sab_ptr_iArray[9]);
    return 0;
}
