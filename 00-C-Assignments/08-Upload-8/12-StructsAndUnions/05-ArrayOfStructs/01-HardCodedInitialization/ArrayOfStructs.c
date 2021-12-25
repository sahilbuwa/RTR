#include<stdio.h>

#define NAME_LENGTH 100
#define MARITAL_STATUS 10

struct sab_Employee
{
    char sab_name[NAME_LENGTH];
    int sab_age;
    float sab_salary;
    char sab_sex;
    char sab_marital_status[MARITAL_STATUS];
};

int main(int argc,char *argv[],char *envp[])
{
    //variable delarations
    struct sab_Employee sab_EmployeeRecord[5];
    char sab_employee_rajesh[]="Rajesh",sab_employee_sameer[]="Sameer",sab_employee_kalyani[]="Kalyani",sab_employee_sonali[]="Sonali",sab_employee_shantanu[]="Shantanu";
    int sab_i;

    //code
    //Initializations
    //Emp 1
    strcpy(sab_EmployeeRecord[0].sab_name,sab_employee_rajesh);
    sab_EmployeeRecord[0].sab_age=30;
    sab_EmployeeRecord[0].sab_sex='M';
    sab_EmployeeRecord[0].sab_salary=50000.0f;
    strcpy(sab_EmployeeRecord[0].sab_marital_status,"Unmarried");
    
    //Emp 2
    strcpy(sab_EmployeeRecord[1].name,sab_employee_sameer);
    sab_EmployeeRecord[1].sab_age=32;
    sab_EmployeeRecord[1].sab_sex='M';
    sab_EmployeeRecord[1].sab_salary=60000.0f;
    strcpy(sab_EmployeeRecord[1].sab_marital_status,"Married");
    
    //Emp 3
    strcpy(sab_EmployeeRecord[2].sab_name,sab_employee_kalyani);
    sab_EmployeeRecord[2].sab_age=29;
    sab_EmployeeRecord[2].sab_sex='F';
    sab_EmployeeRecord[2].sab_salary=62000.0f;
    strcpy(sab_EmployeeRecord[2].marital_status,"Unmarried");
    
    //Emp 4
    strcpy(sab_EmployeeRecord[3].sab_name,sab_employee_sonali);
    sab_EmployeeRecord[3].sab_age=33;
    sab_EmployeeRecord[3].sab_sex='F';
    sab_EmployeeRecord[3].sab_salary=50000.0f;
    strcpy(sab_EmployeeRecord[3].sab_marital_status,"Married");
    
    //Emp 5
    strcpy(sab_EmployeeRecord[4].sab_name,sab_employee_shantanu);
    sab_EmployeeRecord[4].sab_age=35;
    sab_EmployeeRecord[4].sab_sex='M';
    sab_EmployeeRecord[4].sab_salary=55000.0f;
    strcpy(sab_EmployeeRecord[4].sab_marital_status,"Married");
    
    //Display values
    printf("\n\n");
    printf("Employee records\n\n");
    for (sab_i = 0;sab_i < 5;sab_i++)
    {
        printf("****** EMPLOYEE NUMBER %d ******\n\n", (sab_i+1));
        printf("Name        : %s\n",sab_EmployeeRecord[sab_i].sab_name);
        printf("Age         : %d years\n",sab_EmployeeRecord[sab_i].sab_age);

        if (sab_EmployeeRecord[sab_i].sab_sex=='M'||sab_EmployeeRecord[sab_i].sab_sex =='m')
            printf("Sex     :Male\n");
        else
            printf("Sex     :Female\n");

        printf("Salary      :Rs.%f\n",sab_EmployeeRecord[sab_i].sab_salary);
        printf("Lagna status:%s\n",sab_EmployeeRecord[sab_i].sab_marital_status);

        printf("\n\n");

    }
    return 0;

}
