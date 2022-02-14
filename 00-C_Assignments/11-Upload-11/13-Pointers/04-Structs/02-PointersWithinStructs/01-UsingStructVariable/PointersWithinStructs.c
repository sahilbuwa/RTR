#include <stdio.h>

// DEFINING STRUCT
struct sab_MyData
{
    int *sab_ptr_i;
    int sab_i;
    float *sab_ptr_f;
    float sab_f;
    double *sab_ptr_d;
    double sab_d;
};
int main(int argc,char *argv[],char *envp[])
{
    // Variable declarations
    struct sab_MyData sab_data;
    // Code
    sab_data.sab_i = 9;
    sab_data.sab_ptr_i=&sab_data.sab_i;
    sab_data.sab_f = 11.4512f;
    sab_data.sab_ptr_f=&sab_data.sab_f;
    sab_data.sab_d = 30.1219943;
    sab_data.sab_ptr_d = &sab_data.sab_d;
    printf("\n\n");
    printf("i = %d\n", *(sab_data.sab_ptr_i));
    printf("Adress of 'i' = %p\n",sab_data.sab_ptr_i);
    printf("\n\n");
    printf("f = %f\n", *(sab_data.sab_ptr_f));
    printf("Adress of 'f' = %p\n",sab_data.sab_ptr_f);
    printf("\n\n");
    printf("d = %lf\n", *(sab_data.sab_ptr_d));
    printf("Adress of 'd' = %p\n",sab_data.sab_ptr_d);
    return 0;

}
