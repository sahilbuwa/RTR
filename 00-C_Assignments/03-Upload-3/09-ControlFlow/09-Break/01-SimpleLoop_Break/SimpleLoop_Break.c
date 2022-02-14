#include<stdio.h>
int main(void)
{
    //Variable Declarations
    int sab_i;
    char sab_ch;

    //code
    printf("\n\n");

    printf("Printing Even Numbers From 1 to 100 for every user input . exiting the loop when user enters character 'q' or 'Q' : \n\n");
    printf("Enter character 'Q' or 'q' to exit loop: \n\n");

    for(sab_i = 1;sab_i <= 100; sab_i++)
    {
        printf("\t%d\n",sab_i);
        sab_ch=getch();
        if (sab_ch == 'Q' || sab_ch == 'q' )
        {
            break;
        }
    }
    printf("\n\n");
    printf("Exiting loop....");
    printf("\n\n");
    return 0;
}