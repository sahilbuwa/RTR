#include <stdio.h>

// Struct and Union declarations
struct sab_MyStruct
{
    int sab_i;
    float sab_f;
    double sab_d;
    char sab_c;
};
union sab_MyUnion
{
    int sab_i;
    float sab_f;
    double sab_d;
    char sab_c;
};

int main(void)
{
    // variable declarations
    struct sab_MyStruct sab_s;
    union sab_MyUnion sab_u;
    // code
    // Member initializations
    printf("\n\n");
    printf("Members of struct are : \n\n");
    sab_s.sab_i=9;
    sab_s.sab_f=8.7f;
    sab_s.sab_d=1.233422;
    sab_s.sab_c='P';

    // Display
    printf("s.i = %d\n\n",sab_s.sab_i);
    printf("s.f = %f\n\n",sab_s.sab_f);
    printf("s.d = %lf\n\n",sab_s.sab_d);
    printf("s.c = %c\n\n",sab_s.sab_c);

    // Display Addresses
    printf("Addresses of members of struct are: \n\n");
    printf("s.i = %p\n\n",&sab_s.sab_i);
    printf("s.f = %p\n\n",&sab_s.sab_f);
    printf("s.d = %p\n\n",&sab_s.sab_d);
    printf("s.c = %p\n\n",&sab_s.sab_c);

    printf("MyStruct s = %p\n\n",&sab_s);

    // Union initializations
    printf("\n\n");
    printf("Members of union are: \n\n");

    sab_u.sab_i=3;
    printf("u.i = %d\n\n",sab_u.sab_i);
    sab_u.sab_f=4.5f;
    printf("u.f = %f\n\n",sab_u.sab_f);
    sab_u.sab_d=5.12764;
    printf("u.d = %lf\n\n",sab_u.sab_d);
    sab_u.sab_c='A';
    printf("u.c = %c\n\n",sab_u.sab_c);

    //Display Addresses
    printf("Addresses of members of union are: \n\n");
    printf("u.i = %p\n\n",&sab_u.sab_i);
    printf("u.f = %p\n\n",&sab_u.sab_f);
    printf("u.d = %p\n\n",&sab_u.sab_d);
    printf("u.c = %p\n\n",&sab_u.sab_c);

    printf("MyUnion u = %p\n\n",&sab_u);

    return 0;
}
