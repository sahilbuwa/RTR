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

    switch(sab_option)
    {
        case 'A':
        case 'a':
            sab_result = sab_a + sab_b ; 
            printf("Addition of A = %d and B = %d gives result %d !!!\n\n",sab_a,sab_b,sab_result);
            break;
        case 'S':
        case 's':
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
            break;
        case 'M':
        case 'm':
            sab_result = sab_a * sab_b ; 
            printf("Multiplication of A = %d and B = %d gives result %d !!!\n\n",sab_a,sab_b,sab_result);
            break;
        case 'D':
        case 'd':
            printf("Enter Option In Character : \n\n");
            printf("'Q' or 'q' or '/' For Quotient Upon Division : \n");
            printf("'R' or 'r' or '%%' For Remainder Upon Division : \n");

            printf("Enter Option : ");
            sab_option_division = getch();

            printf("\n\n");

            switch(sab_option_division)
            {
                case 'Q':
                case 'q':
                case '/':
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
                    break;
                case 'R':
                case 'r':
                case '%':
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
                    break;
            }
            default:
                printf("Invalid Character %c Entered !!! Please try again...\n\n",sab_option_division);
                break;
    }
    printf("Switch case block complete\n");
    return 0;
}