#include <stdio.h>

#define MAX_NUMBER(a, b) ((a > b) ? a : b)

int main(int argc, char *argv[], char *envp[])
{
    int sab_inum1,sab_inum2,sab_iresult;
    float sab_fnum1,sab_fnum2,sab_fresult;

    //code
    printf("\n\n");
    printf("Enter An Integer Number : \n\n");
    scanf("%d", &sab_inum1);

    printf("\n\n");
    printf("Enter An Integer Number : \n\n");
    scanf("%d", &sab_inum2);

    sab_iresult=MAX_NUMBER(sab_inum1,sab_inum2);
    printf("\n\n");
    printf("Result of the macro function is : %d \n\n",sab_iresult);

    printf("\n\n");

    printf("\n\n");
    printf("Enter An float Number : \n\n");
    scanf("%f", &sab_fnum1);

    printf("\n\n");
    printf("Enter An float Number : \n\n");
    scanf("%f", &sab_fnum2);

    sab_fresult=MAX_NUMBER(sab_fnum1,sab_fnum2);
    printf("\n\n");
    printf("Result of the macro function is : %f \n\n",sab_fresult);

    printf("\n\n");

    return 0;
}