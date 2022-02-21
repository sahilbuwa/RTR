#include <stdio.h>

int main(void)
{
	//variable declarations
	int sab_a = 5;
	int sab_b = 10;

	//code
	printf("\n\n");
	printf("A = %d\n", sab_a);
	printf("A = %d\n", sab_a++);
	printf("A = %d\n", sab_a);
	printf("A = %d\n\n", ++sab_a);

	printf("B = %d\n", sab_b);
	printf("B = %d\n", sab_b--);
	printf("B = %d\n", sab_b);
	printf("B = %d\n\n", --sab_b);

	return(0);
}
