#include <stdio.h>

int main(void)
{
    //function prototypes
    void sab_PrintBinaryFormOfNumber(unsigned int);

    //variable declarations
    unsigned int sab_a;
    unsigned int sab_num_bits;
    unsigned int sab_result;

    //code
    printf("\n\n");
    printf("Enter an integer = ");
    scanf("%u", &sab_a);
    
    printf("\n\n");
    printf("By how many bits do you want to shift A = %d to the right ? ",sab_a);
    scanf("%u", &sab_num_bits);
       
    printf("\n\n\n\n");
    sab_result = sab_a >> sab_num_bits;
    printf("Bitwise Right Shifting A = %d (decimal) by %d (Decimal) gives result %d (Decimal).\n\n",sab_a,sab_num_bits,sab_result);

    sab_PrintBinaryFormOfNumber(sab_a);
    sab_PrintBinaryFormOfNumber(sab_result);

    return 0;
    
}

void sab_PrintBinaryFormOfNumber(unsigned int sab_decimal_number)
{
    //variable declarations
    unsigned int sab_quotient,sab_remainder;
    unsigned int sab_num;
    unsigned int sab_binary_array[8];
    int sab_i;

    //code
    for(sab_i = 0;sab_i < 8; sab_i++)
        sab_binary_array[sab_i] = 0;
    
    printf("The Binary Form Of The Decimal Integer %d Is\t=\t",sab_decimal_number);
    sab_num = sab_decimal_number;
    sab_i = 7;
    while (sab_num!=0)
    {
        sab_quotient = sab_num / 2;
        sab_remainder = sab_num % 2;
        sab_binary_array[sab_i] = sab_remainder;
        sab_num = sab_quotient;
        sab_i--;
    }
    for (sab_i = 0;sab_i < 8; sab_i++)
        printf("%u",sab_binary_array[sab_i]);

    printf("\n\n");
}
