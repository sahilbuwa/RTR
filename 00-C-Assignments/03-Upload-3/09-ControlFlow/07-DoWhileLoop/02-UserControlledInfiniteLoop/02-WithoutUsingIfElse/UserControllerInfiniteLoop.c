#include<stdio.h>
int main (void)
{
    //Variable Declarations
    char sab_option,sab_ch = '\0';

    //code
    printf("\n\n");
    printf("Once The Infinite Loop Begins, Enter 'Q' or 'q' To Quit The InfiniteFor Loop : \n\n");
    
    do 
    {
        do 
        {
            printf("\n");
            printf("In loop...\n");
            sab_ch=getch();
        }while(sab_ch=='Q'||sab_ch=='q');
        printf("\n\n");
        printf("Exitting user controlled loop infinite loop...");
        
        printf("\n\n");
        printf("Do you want to begin user controlled infinite loop again? 'Y' or 'y' To Initiate , any other key for no."); 
        sab_option=getch();
    }while(sab_option=='Y'||sab_option=='y');
    return 0;
}
