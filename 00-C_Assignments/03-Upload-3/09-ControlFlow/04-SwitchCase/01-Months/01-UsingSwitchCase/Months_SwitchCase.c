#include<stdio.h>
int main(void)
{
    //Variable Declarations
    int sab_num_month;
    //code
    printf("\n\n");

    printf("Enter Number Of Month (1 to 12) : ");
    scanf("%d" , &sab_num_month);

    printf("\n\n");

    switch(sab_num_month)
    {
        case 1:
            printf("Month Number %d is January !!!\n\n",sab_num_month);
            break;
        case 2:
            printf("Month Number %d is February !!!\n\n",sab_num_month);
            break;
         case 3:
            printf("Month Number %d is March !!!\n\n",sab_num_month);
            break;
         case 4:
            printf("Month Number %d is April !!!\n\n",sab_num_month);
            break;
         case 5:
            printf("Month Number %d is May !!!\n\n",sab_num_month);
            break;
         case 6:
            printf("Month Number %d is June !!!\n\n",sab_num_month);
            break;
         case 7:
            printf("Month Number %d is July !!!\n\n",sab_num_month);
            break;
         case 8:
            printf("Month Number %d is August !!!\n\n",sab_num_month);
            break;
         case 9:
            printf("Month Number %d is September !!!\n\n",sab_num_month);
            break;
         case 10:
            printf("Month Number %d is October !!!\n\n",sab_num_month);
            break;
         case 11:
            printf("Month Number %d is November !!!\n\n",sab_num_month);
            break;
         case 12:
            printf("Month Number %d is December !!!\n\n",sab_num_month);
            break;
        default:
            printf("Invalid Number %d Entered !!! Please Try Again...\n\n",sab_num_month);
            break;
    }
    printf("Switch case block complete");
    return 0;

}
