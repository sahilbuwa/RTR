#include<stdio.h>
int main(void)
{
    //Variable Declarations
    int sab_i_num,sab_num,sab_i;

    //code
    printf("\n\n");

    printf("Enter an integer value from which iteration must begin : ");
    scanf("%d",&sab_i_num);

    printf("How many digits do you want to print from %d onwards ? : ",sab_i_num);
    scanf("%d",&sab_num);

    printf("Printing digits %d to %d : \n\n",sab_i_num,(sab_i_num+sab_num));

    for(sab_i=sab_i_num;sab_i <= (sab_i_num+sab_num);sab_i++)
    {
        printf("\t%d\n",sab_i);
    }

    printf("\n\n");

    return 0;
}
