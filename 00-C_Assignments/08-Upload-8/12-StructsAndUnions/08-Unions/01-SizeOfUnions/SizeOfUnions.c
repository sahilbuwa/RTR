#include <stdio.h>

//Struct and union definitions

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

int main(int argc, char *argv[], char *envp[])
{
    // variable declarations
    struct sab_MyStruct sab_s;
    union sab_MyUnion sab_u;
    
    // code
    // Printing sizes
    printf("\n\n");
    printf("Size Of MyStruct = %lu\n", sizeof(sab_s));
    printf("\n\n");
    printf("Size Of MyUnion = %lu\n", sizeof(sab_u));

    return 0;
}