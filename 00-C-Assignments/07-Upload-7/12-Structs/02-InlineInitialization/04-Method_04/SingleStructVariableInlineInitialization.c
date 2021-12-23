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
    //code
    struct sab_MyData sab_data_one = { 35 , 3.9f, 1.23765,'A'};
    struct sab_MyData sab_data_two = {'P', 6.2f, 12.199523, 68};
    struct sab_MyData sab_data_three = { 36 ,'G'};
    struct sab_MyData sab_data_four = { 79 };

    //Display values 1
    printf("\n\n");
    printf("Data members of 'struct MyData' are : \n\n");
    printf("i = %d\n",sab_data_one.sab_i);
    printf("f = %f\n",sab_data_one.sab_f);
    printf("d = %lf\n",sab_data_one.sab_d);
    printf("c = %c\n\n",sab_data_one.sab_c);

    //Display values 2
    printf("\n\n");
    printf("Data members of 'struct MyData' are : \n\n");
    printf("i = %d\n",sab_data_two.sab_i);
    printf("f = %f\n",sab_data_two.sab_f);
    printf("d = %lf\n",sab_data_two.sab_d);
    printf("c = %c\n\n",sab_data_two.sab_c);

    //Display values 3
    printf("\n\n");
    printf("Data members of 'struct MyData' are : \n\n");
    printf("i = %d\n",sab_data_three.sab_i);
    printf("f = %f\n",sab_data_three.sab_f);
    printf("d = %lf\n",sab_data_three.sab_d);
    printf("c = %c\n\n",sab_data_three.sab_c);

    //Display values 4
    printf("\n\n");
    printf("Data members of 'struct MyData' are : \n\n");
    printf("i = %d\n",sab_data_four.sab_i);
    printf("f = %f\n",sab_data_four.sab_f);
    printf("d = %lf\n",sab_data_four.sab_d);
    printf("c = %c\n\n",sab_data_four.sab_c);

    return 0;
}
