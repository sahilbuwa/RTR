#include <stdio.h>
int main(void)
{
	//code
    printf("\n\n");
    printf("**************************************************************************************");
    printf("\n\n");

    printf("Hello World !!!\n\n");

	int sab_a = 13;
	printf("Integer Decimal Value Of 'a' = %d\n", sab_a);
	printf("Integer Octal Value Of 'a' = %o\n", sab_a);
	printf("Integer Hexadecimal Value Of 'a' (Hexadecimal Letters In Lower Case) = %x\n", sab_a);
	printf("Integer Hexadecimal Value Of 'a' (Hexadecimal Letters In Lower Case) = %X\n\n", sab_a);

	char sab_ch = 'A';
	printf("Character ch = %c\n", sab_ch);
	char sab_str[] = "AstroMediComp's Real Time Rendering Batch";
	printf("String str = %s\n\n", sab_str);

	long sab_num = 30121995L;
	printf("Long Integer = %ld\n\n", sab_num);

	unsigned int sab_b = 7;
	printf("Unsigned Integer 'b' = %u\n\n", sab_b);

	float sab_f_num = 3012.1995f;
	printf("Floating Point Number With Just %%f 'f_num' = %f\n", sab_f_num);
	printf("Floating Point Number With %%4.2f 'f_num' = %4.2f\n", sab_f_num);
	printf("Floating Point Number With %%6.5f 'f_num' = %6.5f\n\n", sab_f_num);

	double sab_d_pi = 3.14159265358979323846;
	printf("Double Precision Floating Point Number Without Exponential = %g\n", sab_d_pi);
	printf("Double Precision Floating Point Number With Exponential (Lower Case) = %e\n", sab_d_pi);
	printf("Double Precision Floating Point Number With Exponential (Upper Case) = %E\n\n", sab_d_pi);
	printf("Double Hexadecimal Value Of 'd_pi' (Hexadecimal Letters In Lower Case) = %a\n", sab_d_pi);
	printf("Double Hexadecimal Value Of 'd_pi' (Hexadecimal Letters In Upper Case) = %A\n\n", sab_d_pi);
    
    printf("**************************************************************************************\n");
    printf("\n\n");
	return(0);
}
