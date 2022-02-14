#include<stdio.h>

//Defining struct here
struct sab_MyData
{
    int sab_i;
    float sab_f;
    double sab_d;
    char sab_c;
};

int main(int argc, char *argv[], char *envp[])
{
    //variable declarations
    struct sab_MyData sab_data;
    
    //code
    //Input 1
    printf("\n\n");
    printf("Enter integer value for data member 'i' of 'struct MyData':\n");
    scanf("%d", &sab_data.sab_i);

    //Input 2
    printf("Enter floating-point value for data member 'f' of 'struct MyData':\n");
    scanf("%f", &sab_data.sab_f);

    //Input 3
    printf("Enter double value for data member 'd' of 'struct MyData':\n");
    scanf("%lf", &sab_data.sab_d);

    //Input 4
    printf("Enter character value for data member 'c' of 'struct MyData':\n");
    sab_data.sab_c = getch();

    //Display values
    printf("\n\n");
    printf("Data members of 'struct MyData' are : \n\n");
    printf("i = %d\n",sab_data.sab_i);
    printf("f = %f\n",sab_data.sab_f);
    printf("d = %lf\n",sab_data.sab_d);
    printf("c = %c\n\n",sab_data.sab_c);

    return 0;
}
