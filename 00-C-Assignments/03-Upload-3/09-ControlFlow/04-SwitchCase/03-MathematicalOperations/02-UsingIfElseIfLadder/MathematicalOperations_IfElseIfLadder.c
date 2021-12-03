#include<stdio.h>
#include<conio.h>

int main(void)
{
    //Variable Declarations
    int sab_a,sab_b;
    int sab_result;

    char sab_option,sab_option_division;

    //code
    printf("Enter Value For 'A' : ");
    scanf("%d",&sab_a);

    printf("Enter value for 'B' : ");
    scanf("%d",&sab_b);

    printf("Enter Option in character : \n\n");
    printf("'A' or 'a' for addition : \n");
    printf("'S' or 's' for Subtraction : \n");
    printf("'M' or 'm' for Multiplication : \n");
    printf("'D' or 'd' for Division : \n\n");

    printf("Enter Option \n");
    sab_option= getch();

    printf("\n\n");

        if(sab_option=='A'||sab_option=='a')
        {
            sab_result = sab_a + sab_b ; 
            printf("Addition of A = %d and B = %d gives result %d !!!\n\n",sab_a,sab_b,sab_result);
            
        }
        else if (sab_option=='S' || sab_option=='s')
        {
            if(sab_a >= sab_b)
            {
                sab_result = sab_a - sab_b ; 
                printf("Subtraction of A = %d and B = %d gives result %d !!!\n\n",sab_a,sab_b,sab_result);
            }
            else
            {
                sab_result = sab_b - sab_a ; 
                printf("Subtraction of A = %d and B = %d gives result %d !!!\n\n",sab_a,sab_b,sab_result);
            }
        }
        else if(sab_option=='M'||sab_option=='m')
        {
            sab_result = sab_a * sab_b ; 
            printf("Multiplication of A = %d and B = %d gives result %d !!!\n\n",sab_a,sab_b,sab_result);
        }
        else if(sab_option=='D'||sab_option=='d')
        {
            printf("Enter Option In Character : \n\n");
            printf("'Q' or 'q' or '/' For Quotient Upon Division : \n");
            printf("'R' or 'r' or '%%' For Remainder Upon Division : \n");

            printf("Enter Option : ");
            sab_option_division = getch();

            printf("\n\n");

            if(sab_option_division=='Q'||sab_option_division=='q'||sab_option_division=='/')
            {
                    if(sab_a>=sab_b)
                    {
                        sab_result = sab_a / sab_b ; 
                        printf("Division of A = %d and B = %d gives result %d !!!\n\n",sab_a,sab_b,sab_result);
                    }   
                    else
                    {
                        sab_result = sab_b / sab_a ; 
                        printf("Division of A = %d and B = %d gives result %d !!!\n\n",sab_a,sab_b,sab_result);
                    }         
            }
            else if(sab_option_division=='R'||sab_option_division=='r'||sab_option_division=='%')
            {
                    if(sab_a>=sab_b)
                    {
                        sab_result = sab_a % sab_b ; 
                        printf("Division of A = %d and B = %d gives result %d !!!\n\n",sab_a,sab_b,sab_result);
                    }   
                    else
                    {
                        sab_result = sab_b % sab_a ; 
                        printf("Division of A = %d and B = %d gives result %d !!!\n\n",sab_a,sab_b,sab_result);
                    }         
                    
            }
        }
        else 
            printf("Invalid Character %c Entered !!! Please try again...\n\n");
    return 0;
}