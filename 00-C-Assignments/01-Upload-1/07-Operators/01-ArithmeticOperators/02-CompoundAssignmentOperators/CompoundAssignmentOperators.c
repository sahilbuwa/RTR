#include <stdio.h>

int main(void)
{
	//variable declarations
	int sab_a;
	int sab_b;
	int sab_x;

	//code
	printf("\n\n");
	printf("Enter A Number : ");
	scanf("%d", &sab_a);

	printf("\n\n");
	printf("Enter Another Number : ");
	scanf("%d", &sab_b);

	printf("\n\n");

	//Since, In All The Following 5 Cases, The Operand on The Left 'a' Is Getting Repeated Immeiately On The Right (e.g : a = a + b or a = a -b),
	//We Are Using Compound Assignment Operators +=, -=, *=, /= and %=

	//Since, 'a' Will Be Assigned The Value Of (a + b) At The Expression (a += b), We Must Save The Original Value Of 'a' To Another Variable (x)
	sab_x = sab_a;
	sab_a += sab_b; // a = a + b
	printf("Addition Of A = %d And B = %d Gives %d.\n", sab_x, sab_b, sab_a);

	//Value Of 'a' Altered In The Above Expression Is Used Here...
	//Since, 'a' Will Be Assigned The Value Of (a - b) At The Expression (a -= b), We Must Save The Original Value Of 'a' To Another Variable (x)
	sab_x = sab_a;
	sab_a -= sab_b; // a = a - b
	printf("Subtraction Of A = %d And B = %d Gives %d.\n", sab_x, sab_b, sab_a);

	//Value Of 'a' Altered In The Above Expression Is Used Here...
	//Since, 'a' Will Be Assigned The Value Of (a * b) At The Expression (a *= b), We Must Save The Original Value Of 'a' To Another Variable (x)
	sab_x = sab_a;
	sab_a *= sab_b; // a = a * b
	printf("Multiplication Of A = %d And B = %d Gives %d.\n", sab_x, sab_b, sab_a);

	//Value Of 'a' Altered In The Above Expression Is Used Here...
	//Since, 'a' Will Be Assigned The Value Of (a / b) At The Expression (a /= b), We Must Save The Original Value Of 'a' To Another Variable (x)
	sab_x = sab_a;
	sab_a /= sab_b; // a = a / b 
	printf("Division Of A = %d And B = %d Gives Quotient %d.\n", sab_x, sab_b, sab_a);

	//Value Of 'a' Altered In The Above Expression Is Used Here...
	//Since, 'a' Will Be Assigned The Value Of (a % b) At The Expression (a %= b), We Must Save The Original Value Of 'a' To Another Variable (x)
	sab_x = sab_a;
	sab_a %= sab_b; // a = a % b
	printf("Division Of A = %d And B = %d Gives Remainder %d.\n", sab_x, sab_b, sab_a);

	printf("\n\n");

	return(0);
}
