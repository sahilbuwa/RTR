#include<stdio.h>
int main (void)
{
    //Variable Declarations
    char sab_option,sab_ch = '\0';

    //code
    printf("\n\n");
    printf("Once The Infinite Loop Begins, Enter 'Q' or 'q' To Quit The InfiniteFor Loop : \n\n");
    printf("Enter 'Y' oy 'y' To Initiate User Controlled Infinite Loop : ");
    printf("\n\n");
    sab_option = getch();
    if(sab_option=='Y'||sab_option=='y')
    {
        for(;;)
        {
            printf("In loop...\n");
            sab_ch=getch();
            if(sab_ch=='Q'||sab_ch=='q')
                break;
        }
    }
    printf("\n\n");
    printf("Exitting user controlled loop infinite loop...");
    printf("\n\n");
    return 0;
}