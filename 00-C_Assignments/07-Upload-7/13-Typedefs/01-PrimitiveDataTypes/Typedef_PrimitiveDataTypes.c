#include <stdio.h>

//global typedef
typedef int sab_MY_INT;

int main(void)
{
    //function prototype
    sab_MY_INT sab_Add(sab_MY_INT,sab_MY_INT);
    //typedefs of primitive types
    typedef int sab_MY_INT; 

    typedef float sab_MY_FLOAT;

    typedef char sab_MY_CHARACTER; 

    typedef double sab_MY_DOUBLE;
    //typedefs of win32 datatypes
    typedef unsigned int sab_UINT;
    typedef sab_UINT sab_HANDLE;
    typedef sab_HANDLE sab_HWND;
    typedef sab_HANDLE sab_HINSTANCE;

    // variable declarations
    //Ints
    sab_MY_INT sab_a = 10, sab_i;
    sab_MY_INT sab_iArray[] = {9, 18, 27, 36, 45, 54, 63, 72, 81, 90};
    //Floats
    sab_MY_FLOAT sab_f = 30.9f;
    const sab_MY_FLOAT sab_f_pi = 3.1415f;
    //Chars
    sab_MY_CHARACTER sab_ch = '*';
    sab_MY_CHARACTER sab_chArray_01[] = "Hello";
    sab_MY_CHARACTER sab_chArray_02[][10] = { "RTR", "Batch", "2021-2022"};
    //Doubles
    sab_MY_DOUBLE sab_d = 8.04198;

    //Win32s
    sab_UINT sab_uint = 3456;
    sab_HANDLE sab_handle = 12987;
    sab_HWND sab_hwnd = 9876;
    sab_HINSTANCE sab_hInstance = 14466;

    // code
    //Display all values
    printf("\n\n");
    printf("MY_INT variable a=%d\n",sab_a);

    printf("\n\n");

    for (sab_i = 0; sab_i < (sizeof(sab_iArray)/sizeof(int)); sab_i++)
    {
        printf("MY_INT array variable iArray[ %d ]= %d\n", sab_i, sab_iArray[sab_i]);
    }

    printf("\n\n");

    printf("\n\n");
    printf("MY_FLOAT variable f = %f\n", sab_f);
    printf("MY_FLOAT constanct f_pi = %f\n", sab_f_pi);

    printf("\n\n");
    printf("MY_DOUBLE variable d = %lf\n", sab_d);

    printf("\n\n");
    printf("MY_CHARACTER variable ch = %c\n", sab_ch);

    printf("\n\n");
    printf("MY_CHARACTER array variable chArray_01 = %s\n", sab_chArray_01);

    printf("\n\n");

    for (sab_i = 0; sab_i < (sizeof(sab_chArray_02)/sizeof(sab_chArray_02[0])); sab_i++)
    {
        printf("%s\t", sab_chArray_02[sab_i]);
    }

    printf("\n\n");

    printf("\n\n");
    printf("UINT variable uint = %u\n\n", sab_uint);
    printf("HANDLE variable handle = %u\n\n", sab_handle);
    printf("HWND variable hwnd = %u\n\n", sab_hwnd);
    printf("HINSTANCE variable hinstance = %u\n\n", sab_hInstance);
    printf("\n\n");

    sab_MY_INT sab_x = 91;
    sab_MY_INT sab_y = 31;
    sab_MY_INT sab_ret;

    sab_ret = sab_Add(sab_x, sab_y);
    printf("ret = %d\n\n", sab_ret);

    return 0;
}
sab_MY_INT sab_Add(sab_MY_INT sab_a, sab_MY_INT sab_b)
{
    // code
    sab_MY_INT sab_c;
    sab_c = sab_a + sab_b;
    return sab_c;
}
