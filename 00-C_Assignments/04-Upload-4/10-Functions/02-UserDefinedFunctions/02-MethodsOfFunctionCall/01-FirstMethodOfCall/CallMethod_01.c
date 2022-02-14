#include <stdio.h> 

int main(int argc, char *argv[], char *envp[])
{
    //function prototypes
    void sab_MyAddition(void);
    int sab_MySubtraction(void);
    void sab_MyMultiplication(int, int);
    int sab_MyDivision(int, int);
    //variable declarations
    int sab_result_subtraction;
    int sab_a_multiplication, sab_b_multiplication;
    int sab_a_division, sab_b_division, sab_result_division;
    //code
    sab_MyAddition(); 
    
    sab_result_subtraction = sab_MySubtraction(); 
    printf("\n\n");
    printf("Subtraction Yields Result = %d\n", sab_result_subtraction);

    printf("\n\n");
    printf("Enter Integer Value For 'A' For Multiplication : ");
    scanf("%d", &sab_a_multiplication);
    printf("\n\n");
    printf("Enter Integer Value For 'B' For Multiplication : ");
    scanf("%d", &sab_b_multiplication);
    sab_MyMultiplication(sab_a_multiplication, sab_b_multiplication); 
    
    printf("\n\n");
    printf("Enter Integer Value For 'A' For Division : ");
    scanf("%d", &sab_a_division);
    printf("\n\n");
    printf("Enter Integer Value For 'B' For Division : ");
    scanf("%d", &sab_b_division);
    sab_result_division = sab_MyDivision(sab_a_division, sab_b_division);
    printf("\n\n");
    printf("Division Of %d and %d Gives = %d (Quotient)\n", sab_a_division,sab_b_division, sab_result_division);
    printf("\n\n");
    return 0;
}

void sab_MyAddition(void) 
{
    //variable declarations
    int sab_a, sab_b, sab_sum;
    //code
    printf("\n\n");
    printf("Enter Integer Value For 'A' For Addition : ");
    scanf("%d", &sab_a);
    printf("\n\n");
    printf("Enter Integer Value For 'B' For Addition : ");
    scanf("%d", &sab_b);
    sab_sum = sab_a + sab_b;
    printf("\n\n");
    printf("Sum Of %d And %d = %d\n\n", sab_a, sab_b, sab_sum);
}

int sab_MySubtraction(void) 
{
    //variable declarations 
    int sab_a, sab_b, sab_subtraction;
    //code
    printf("\n\n");
    printf("Enter Integer Value For 'A' For Subtraction : ");
    scanf("%d", &sab_a);
    printf("\n\n");
    printf("Enter Integer Value For 'B' For Subtraction : ");
    scanf("%d", &sab_b);
    sab_subtraction = sab_a - sab_b;
    return(sab_subtraction);
}

void sab_MyMultiplication(int sab_a, int sab_b)
{
    int sab_multiplication;
    //code
    sab_multiplication = sab_a * sab_b;
    printf("\n\n");
    printf("Multiplication Of %d And %d = %d\n\n", sab_a, sab_b, sab_multiplication);
}

int sab_MyDivision(int sab_a, int sab_b)
{
    //variable declarations
    int sab_division_quotient;
    //code
    if (sab_a > sab_b)
        sab_division_quotient = sab_a / sab_b;
    else
        sab_division_quotient = sab_b / sab_a;
    return(sab_division_quotient);
}
