#include<stdio.h>

//Defining struct here
struct sab_MyData
{
    int sab_i;
    float sab_f;
    double sab_d;
    char sab_c;
};

struct sab_MyData sab_data;

int main(int argc, char *argv[], char *envp[])
{
    //Variable Declarations
    int sab_i_size,sab_f_size,sab_d_size, sab_struct_MyData_size;

    //code
    //Assign values
    sab_data.sab_i = 30;
    sab_data.sab_f = 11.45f;
    sab_data.sab_d = 1.2995;

    //Display values
    printf("\n\n");
    printf("Data members of 'struct MyData' are : \n\n");
    printf("i = %d\n",sab_data.sab_i);
    printf("f = %f\n",sab_data.sab_f);
    printf("d = %lf\n",sab_data.sab_d);

    //Calculate sizes
    sab_i_size = sizeof(sab_data.sab_i);
    sab_f_size = sizeof(sab_data.sab_f);
    sab_d_size = sizeof(sab_data.sab_d);

    //Display sizes
    printf("\n\n");
    printf("Sizes of data members of 'struct MyData' are : \n\n");
    printf("Size of 'i' = %d bytes\n",sab_i_size);
    printf("Size of 'f' = %d bytes\n",sab_f_size);
    printf("Size of 'd' = %d bytes\n",sab_d_size);

    //Calculate sizes
    sab_struct_MyData_size = sizeof(struct sab_MyData);

    //Display sizes
    printf("\n\n");
    printf("Size of 'struct MyData' : %d bytes\n\n",sab_struct_MyData_size);

    return 0;
}
