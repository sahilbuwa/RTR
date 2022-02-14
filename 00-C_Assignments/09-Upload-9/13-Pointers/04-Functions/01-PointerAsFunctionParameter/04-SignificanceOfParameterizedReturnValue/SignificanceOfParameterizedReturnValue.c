#include <stdio.h>

enum
{
    NEGATIVE = -1,
    ZERO,
    POSITIVE
};

int main(int argc,char *argv[],char *envp[])
{
    // Function declarations
    int sab_Difference(int, int, int *);
    // Variable declaration
    int sab_a;
    int sab_b;
    int sab_answer, sab_ret;
    // Code
    printf("\n\n");
    printf("Enter value of 'A' : ");
    scanf("%d", &sab_a);

    printf("\n\n");
    printf("Enter value of 'B' : ");
    scanf("%d", &sab_b);

    sab_ret = sab_Difference(sab_a,sab_b,&sab_answer);

    printf("\n\n");
    printf("Difference of %d and %d = %d\n\n",sab_a,sab_b,sab_answer);

    if(sab_ret == POSITIVE)
        printf("The difference of %d and %d is positive !!!\n\n",sab_a,sab_b);
    else if(sab_ret == NEGATIVE)
        printf("The difference of %d and %d is negative !!!\n\n",sab_a,sab_b);
    else
        printf("The difference of %d and %d is zero !!!\n\n",sab_a,sab_b);

    return 0;
}

int sab_Difference(int sab_x,int sab_y,int *sab_diff)
{
    //code
    *sab_diff =sab_x -sab_y;
    if(*sab_diff > 0)
        return(POSITIVE);
    else if (*sab_diff < 0)
        return(NEGATIVE);
    else
        return(ZERO);
}
