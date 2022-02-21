#include<stdio.h>

//Defining struct here
struct sab_MyData
{
    int sab_i;
    float sab_f;
    double sab_d;
    char sab_c;
} sab_data = {30 , 4.5f , 11.451995 , 'A'};//Assign Values

int main(int argc, char *argv[], char *envp[])
{
    //code
    //Display values
    printf("\n\n");
    printf("Data members of 'struct MyData' are : \n\n");
    printf("i = %d\n",sab_data.sab_i);
    printf("f = %f\n",sab_data.sab_f);
    printf("d = %lf\n",sab_data.sab_d);
    printf("c = %c\n\n",sab_data.sab_c);

    return 0;
}
