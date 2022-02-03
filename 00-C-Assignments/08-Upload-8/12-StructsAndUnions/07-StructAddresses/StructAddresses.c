#include <stdio.h>
//Struct Declarations
struct sab_MyData
{
    int sab_i;
    float sab_f;
    double sab_d;
    char sab_c;
};
int main(int argc,char *argv[],char *envp[])
{
    //variable declarations
    struct sab_MyData sab_data;
    //code
    //Initializations
    sab_data.sab_i = 30;
    sab_data.sab_f = 11.45f;
    sab_data.sab_d = 1.2995;
    sab_data.sab_c = 'A';
    //Display values
    printf("\n\n");
    printf("Data members of 'struct MyData' are:\n\n");
    printf("i =%d\n",  sab_data.sab_i);
    printf("f =%f\n",  sab_data.sab_f);
    printf("d =%lf\n", sab_data.sab_d);
    printf("c =%c\n\n",sab_data.sab_c);
    printf("\n\n");
    printf("Addresses of'struct MyData' are:\n\n");
    printf("'i' occupies addresses from %p\n",&sab_data.sab_i);
    printf("'f' occupies addresses from %p\n",&sab_data.sab_f);
    printf("'d' occupies addresses from %p\n",&sab_data.sab_d);
    printf("'c' occupies address %p\n\n",&sab_data.sab_c);
    printf("Starting address of 'struct MyData' variable data=%p\n\n",&sab_data);
    return 0;

}
