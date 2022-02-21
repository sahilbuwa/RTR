#include <stdio.h>

#define MAX_NAME_LENGTH 110

//Struct global declarations
struct sab_Employee
{
    char sab_name[MAX_NAME_LENGTH];
    unsigned int sab_age;
    char sab_gender;
    double sab_salary;
};

struct sab_MyData
{
    int sab_i;
    float sab_f;
    double sab_d;
    char sab_c;
};

int main(void)
{
    // Typedefs of structs
    typedef struct sab_Employee sab_MY_EMPLOYEE_TYPE;
    typedef struct sab_MyData sab_MY_DATA_TYPE;

    // variable declarations and initializations
    struct sab_Employee sab_emp = { "Money", 25, 'M', 10000.00};
    sab_MY_EMPLOYEE_TYPE sab_emp_typedef = { "Zunny", 21, 'F', 20000.00};

    struct sab_MyData sab_md = { 30, 11.45f, 26.122017, 'S'};
    sab_MY_DATA_TYPE sab_md_typedef;

    // code
    sab_md_typedef.sab_i = 9;
    sab_md_typedef.sab_f = 1.5f;
    sab_md_typedef.sab_d = 8.041997;
    sab_md_typedef.sab_c = 'S';

    //Display values 1.1
    printf("\n\n");
    printf("struct Employee : \n\n");
    printf("emp.name = %s\n",    sab_emp.sab_name);
    printf("emp.age = %d\n",     sab_emp.sab_age);
    printf("emp.gender = %c\n",  sab_emp.sab_gender);
    printf("emp.salary = %lf\n", sab_emp.sab_salary);

    //Display values 1.2
    printf("\n\n");
    printf("MY_EMPLOYEE_TYPE : \n\n");
    printf("emp_typedef.name = %s\n",   sab_emp_typedef.sab_name);
    printf("emp_typedef.age = %d\n",    sab_emp_typedef.sab_age);
    printf("emp_typedef.gender = %c\n", sab_emp_typedef.sab_gender);
    printf("emp_typedef.salary = %lf\n",sab_emp_typedef.sab_salary);

    //Display values 2.1
    printf("\n\n");
    printf("struct MyData : \n\n");
    printf("md.i = %d\n", sab_md.sab_i);
    printf("md.f = %f\n", sab_md.sab_f);
    printf("md.d = %lf\n",sab_md.sab_d);
    printf("md.c = %c\n", sab_md.sab_c);
    
    //Display values 2.2
    printf("\n\n");
    printf("MY_DATA_TYPE : \n\n");
    printf("md_typedef.i = %d\n", sab_md_typedef.sab_i);
    printf("md_typedef.f = %f\n", sab_md_typedef.sab_f);
    printf("md_typedef.d = %lf\n",sab_md_typedef.sab_d);
    printf("md_typedef.c = %c\n", sab_md_typedef.sab_c);

    printf("\n\n");

    return 0;
}
