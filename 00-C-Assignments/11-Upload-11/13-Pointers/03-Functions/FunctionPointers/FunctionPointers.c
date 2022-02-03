#include<stdio.h>

int main(int argc,char *argv[],char *envp[])
{
    // Function Declarations
    int sab_AddIntegers(int,int);
    int sab_SubtractIntegers(int,int);
    float sab_AddFloats(int,int);

    // Variable Declarations
    typedef int (*sab_AddIntsFnPtr)(int,int);
    sab_AddIntsFnPtr sab_ptrAddTwoIntegers=NULL;
    sab_AddIntsFnPtr sab_ptrFunc=NULL;

    typedef float (*sab_AddFloatsFnPtr)(float,float);
    sab_AddFloatsFnPtr sab_ptrAddTwoFloats=NULL;

    int sab_iAnswer=0;
    float sab_fAnswer = 0.0f;

    // Code
    sab_ptrAddTwoIntegers = sab_AddIntegers;
    sab_iAnswer = sab_ptrAddTwoIntegers(10,30);
    printf("\n\n");
    printf("Sum of integers = %d\n\n",sab_iAnswer);

    sab_ptrFunc=sab_SubtractIntegers;
    sab_iAnswer=sab_ptrFunc(30,10);
    printf("\n\n");
    printf("Subtraction of integers = %d\n\n",sab_iAnswer);

    sab_ptrAddTwoFloats=sab_AddFloats;
    sab_fAnswer=sab_ptrAddTwoFloats(11.45f,8.2f);
    printf("\n\n");
    printf("Sum of floating point numbers = %f\n\n ",sab_fAnswer);

    return 0;
}

int sab_AddIntegers(int sab_a,int sab_b)
{
    // Variable Declarations
    int sab_c;
    // Code
    sab_c = sab_a +sab_b;
    return sab_c;
}

int sab_SubtractIntegers(int sab_a,int sab_b)
{
    // Variable Declarations
    int sab_c;
    // Code
    if(sab_a>sab_b)
        sab_c= sab_a - sab_b;
    else
        sab_c=sab_a-sab_b;

    return sab_c;

}

float sab_AddFloats(float sab_fnum1,float sab_fnum2)
{
    // Variable Dclarations
    float sab_ans;

    // Code
    sab_ans = sab_fnum1+sab_fnum2;
    return sab_ans;
}
