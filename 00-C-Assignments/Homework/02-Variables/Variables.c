#include <stdio.h>

int main(void)
{

    // variable declarations
    int sab_i = 5;
    float sab_f = 3.9f;
    double sab_d = 8.041997;
    char sab_c = 'A';
    
	//code
    printf("\n\n");
    
    printf("i = %d\n", sab_i);
    printf("f = %f\n", sab_f);
    printf("d = %lf\n", sab_d);
    printf("c = %c\n", sab_c);

    printf("\n\n");

    sab_i = 43;
    sab_f = 6.54f;
    sab_d = 26.1294;
    sab_c = 'P';
    
    printf("i = %d\n", sab_i);
    printf("f = %f\n", sab_f);
    printf("d = %lf\n", sab_d);
    printf("c = %c\n", sab_c);

    printf("\n\n");

    return(0);
}



