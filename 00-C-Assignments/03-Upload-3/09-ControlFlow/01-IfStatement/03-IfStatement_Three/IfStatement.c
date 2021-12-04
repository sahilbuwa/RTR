#include<stdio.h>
int main(void)
{
    //Variable Declarations
    int sab_num;

    //code
    printf("\n\n");

    printf("Enter Value For 'num' : ");
    scanf("%d",&sab_num);

    if (sab_num<0)
    {
        printf("Num = %d is less than 0.(Negative)\n\n",sab_num);
    }
    if ((sab_num>0)&&(sab_num<=100))
    {
        printf("Num = %d is between 0 and 100.\n\n",sab_num);
    }
    if ((sab_num>100)&&(sab_num<=200))
    {
        printf("Num = %d is between 100 and 200.\n\n",sab_num);
    }
    if ((sab_num>200)&&(sab_num<=300))
    {
        printf("Num = %d is between 200 and 300.\n\n",sab_num);
    }
    if ((sab_num>300)&&(sab_num<=400))
    {
        printf("Num = %d is between 300  and 400.\n\n",sab_num);
    }
    if ((sab_num>400)&&(sab_num<=500))
    {
        printf("Num = %d is between 400 and 500.\n\n",sab_num);
    }
    if ((sab_num>500))
    {
        printf("Num = %d is greater than 500.\n\n",sab_num);
    }
    return 0;

}