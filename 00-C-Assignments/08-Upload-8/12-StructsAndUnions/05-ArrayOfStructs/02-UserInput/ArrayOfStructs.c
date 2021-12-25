#include<stdio.h>
#include<ctype.h>

#define NUM_EMPLOYEES 5
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
    //function declarations
    void sab_MyGetString(char[],int);
    //variable delarations
    struct sab_Employee sab_EmployeeRecord[NUM_EMPLOYEES];
    int sab_i;

    //code
    //Initializations : User input
    for (sab_i=0;sab_i<NUM_EMPLOYEES;sab_i++)
    {
        printf("\n\n\n\n");
        printf("Employee number %d\n",(sab_i+1));

        printf("\n\n");
        printf("Enter employee name: ");
        sab_MyGetString(sab_EmployeeRecord[sab_i].sab_name,NAME_LENGTH);

        printf("\n\n\n");
        printf("Enter employee's age(in years): ");
        scanf("%d",&sab_EmployeeRecord[sab_i].sab_age);

        printf("\n\n");
        printf("Enter employee's sex( M/m for male,F/f for female ): ");
        sab_EmployeeRecord[sab_i].sab_sex = getch();
        printf("%c",sab_EmployeeRecord[sab_i].sab_sex);
        sab_EmployeeRecord[sab_i].sab_sex=toupper(sab_EmployeeRecord[sab_i].sab_sex);

        printf("\n\n\n");
        printf("Enter employee's salary(in Indian rupees): ");
        scanf("%f",&sab_EmployeeRecord[sab_i].sab_salary);

        printf("\n\n");
        printf("Is the employee married?(Y/y for yes,N/n for no): ");
        sab_EmployeeRecord[sab_i].sab_marital_status = getch();

        printf("%c",sab_EmployeeRecord[sab_i].sab_marital_status);

        sab_EmployeeRecord[sab_i].sab_marital_status=toupper(sab_EmployeeRecord[sab_i].sab_marital_status);
    }
    
    //Display values
    printf("\n\n");
    printf("Employee records\n\n");
    for (sab_i = 0;sab_i < 5;sab_i++)
    {
        printf("Employee number %d\n\n", (sab_i+1));
        printf("Name        : %s\n",sab_EmployeeRecord[sab_i].sab_name);
        printf("Age         : %d years\n",sab_EmployeeRecord[sab_i].sab_age);

        if (sab_EmployeeRecord[sab_i].sab_sex=='M')
            printf("Sex         :Male\n");
        else if(sab_EmployeeRecord[sab_i].sab_sex=='F')
            printf("Sex         :Female\n");
        else
            printf("Sex         :Invalid data entered\n");

        printf("Salary      :Rs.%f\n",sab_EmployeeRecord[sab_i].sab_salary);

        if(sab_EmployeeRecord[sab_i].sab_marital_status=='Y')
            printf("Lagna status: Married\n");

        else if(sab_EmployeeRecord[sab_i].sab_marital_status=='N')
            printf("Lagna status: Unmarried\n");

        else
            printf("Lagna status: Invalid data entered\n");

        printf("\n\n");

    }
    return 0;

}

void sab_MyGetString(char sab_str[], int sab_str_size)
{
    int i;
    char sab_ch = '\0';
    //code
    i = 0;
    do
    {
        sab_ch = getch();
        sab_str[i]=sab_ch;
        printf("%c",sab_str[i]);
        i++;
    }while ((sab_ch !='\r')&&(i < sab_str_size));
    if (i == sab_str_size)
        sab_str[i-1] ='\0';
    else
        sab_str[i] ='\0';
}
