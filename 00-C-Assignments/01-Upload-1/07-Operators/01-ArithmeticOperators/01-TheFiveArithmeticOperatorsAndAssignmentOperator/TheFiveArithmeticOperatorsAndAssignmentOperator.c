#include <stdio.h>

int main(void)
{
	//variable declarations
	int sab_a;
	int sab_b;
	int sab_result;

	//code
	printf("\n\n");
	printf("Enter A Number : ");
	scanf("%d", &sab_a);

	printf("\n\n");
	printf("Enter Another Number : ");
	scanf("%d", &sab_b);

	printf("\n\n");

	// *** The Following Are The 5 Arithmetic Operators +, -, *, / and % ***
	// *** Also, The Resultants Of The Arithmetic Operations In All The Below Five Cases Have Been Assigned To The Variable 'result' Using the Assignment Operator (=) ***
	sab_result = sab_a + sab_b;
	printf("Addition Of A = %d And B = %d Gives %d.\n", sab_a, sab_b, sab_result);

	sab_result = sab_a - sab_b;
	printf("Subtraction Of A = %d And B = %d Gives %d.\n", sab_a, sab_b, sab_result);

	sab_result = sab_a * sab_b;
	printf("Multiplication Of A = %d And B = %d Gives %d.\n", sab_a, sab_b, sab_result);

	sab_result = sab_a / sab_b;
	printf("Division Of A = %d And B = %d Gives Quotient %d.\n", sab_a, sab_b, sab_result);

	sab_result = sab_a % sab_b;
	printf("Division Of A = %d And B = %d Gives Remainder %d.\n", sab_a, sab_b, sab_result);

	printf("\n\n");

	return(0);
}
