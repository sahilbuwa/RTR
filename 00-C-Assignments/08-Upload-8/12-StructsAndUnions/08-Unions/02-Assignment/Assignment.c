#include <stdio.h>
// Union Declarations
union sab_MyUnion
{
    int sab_i;
    float sab_f;
    double sab_d;
    char sab_c;
};

int main(int argc, char *argv[], char *envp[])
{
    //variable declarations
    union sab_MyUnion sab_u1,sab_u2;
    //code
    //1st union
    printf("\n\n");
    printf("Members of union u1 are: \n\n");

    sab_u1.sab_i=6;
    sab_u1.sab_f=1.2f;
    sab_u1.sab_d=8.333333;
    sab_u1.sab_c='S';

    printf("u1.i =%d\n\n",sab_u1.sab_i);
    printf("u1.f =%f\n\n",sab_u1.sab_f);
    printf("u1.d =%lf\n\n",sab_u1.sab_d);
    printf("u1.c =%c\n\n",sab_u1.sab_c);

    printf("Addresses of members of union u1 are :\n\n");
    printf("u1.i = %p\n\n",&sab_u1.sab_i);
    printf("u1.f = %p\n\n",&sab_u1.sab_f);
    printf("u1.d = %p\n\n",&sab_u1.sab_d);
    printf("u1.c = %p\n\n",&sab_u1.sab_c);


    printf("MyUnion u1 = %p\n\n",&sab_u1);

    //2nd union
    printf("\n\n");
    printf("Members of union u2 are : \n\n");

    sab_u2.sab_i=3;
    printf("u2.i = %d\n\n",sab_u2.sab_i);
    sab_u2.sab_f=4.5f;
    printf("u2.f = %f\n\n",sab_u2.sab_f);
    sab_u2.sab_d=5.12764;
    printf("u2.d = %lf\n\n",sab_u2.sab_d);
    sab_u2.sab_c ='A';
    printf("u2.c = %c\n\n",sab_u2.sab_c);

    printf("Addresses of members of union u2 are : \n\n");
    printf("u2.i = %p\n\n",&sab_u2.sab_i);
    printf("u2.f = %p\n\n",&sab_u2.sab_f);
    printf("u2.d = %p\n\n",&sab_u2.sab_d);
    printf("u2.c = %p\n\n",&sab_u2.sab_c);

    printf("MyUnion u2 = %p\n\n",&sab_u2);

    return 0;
}
