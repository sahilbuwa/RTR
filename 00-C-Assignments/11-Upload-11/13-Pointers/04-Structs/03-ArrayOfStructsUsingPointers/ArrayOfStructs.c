#include <stdio.h>
#include <ctype.h>

#define NAME_LENGTH 100
#define MARITAL_STATUS 10

//Struct Declarations
struct sab_Employee
{
    char sab_name[NAME_LENGTH];
    int sab_age;
    char sab_sex;
    float sab_salary;
    char sab_marital_status;
};

int main(int argc,char *argv[],char *envp[])
{
    // Function prototype
    void sab_MyGetString(char[], int);
    // Variable delarations
    struct sab_Employee *sab_pEmployeeRecord=NULL;
    int sab_num_employees,i;
    // Code

    printf("\n\n");
    printf("Enter number of employees whose details you want to record: ");
    scanf("%d",&sab_num_employees);
    printf("\n\n");

    sab_pEmployeeRecord=(struct sab_Employee *)malloc(sizeof(struct sab_Employee)*sab_num_employees);
    if(sab_pEmployeeRecord == NULL)
    {
        printf("Failed to allocate memory to struct!!!Exiting now...\n\n",sab_num_employees);
        exit(0);
    }
    else
        printf("Successfully allocated memory to %d employees!!!\n\n",sab_num_employees);
    
    
    for(i=0;i<sab_num_employees;i++)
    {
        printf("\n\n\n\n");
        printf("*******Data entry for employee number %d ******\n",(i+1));
        
        printf("\n\n");
        printf("Enter employee name: ");
        sab_MyGetString(sab_pEmployeeRecord[i].sab_name,NAME_LENGTH);
        
        printf("\n\n\n");
        printf("Enter employee's age(in years): ");
        scanf("%d",&sab_pEmployeeRecord[i].sab_age);
        
        printf("\n\n");
        printf("Enter employee's sex (M/m) for male,F/f for female) : ");
        sab_pEmployeeRecord[i].sab_sex=getch();
        printf("%c",sab_pEmployeeRecord[i].sab_sex);
        sab_pEmployeeRecord[i].sab_sex=toupper(sab_pEmployeeRecord[i].sab_sex);
        
        printf("\n\n\n");
        printf("Enter employees salary(in Indian rupees): ");
        scanf("%f",&sab_pEmployeeRecord[i].sab_salary);

        printf("\n\n");
        printf("Is the employee married?(Y/y for yes, N/n for no): ");
        sab_pEmployeeRecord[i].sab_marital_status=getch();
        printf("%c",sab_pEmployeeRecord[i].sab_marital_status);
        sab_pEmployeeRecord[i].sab_marital_status=toupper(sab_pEmployeeRecord[i].sab_marital_status);
    }


    printf("\n\n\n\n");
    printf("****Displaying employee records *******\n\n");

    for(i=0;i<sab_num_employees;i++)
    {
        printf("*****Employee number %d ******\n\n",(i+1));
        printf("Name : %s\n",sab_pEmployeeRecord[i].sab_name);
        printf("Age : %d years\n",sab_pEmployeeRecord[i].sab_age);

        if(sab_pEmployeeRecord[i].sab_sex=='M')
            printf("Sex :male\n");
        else if(sab_pEmployeeRecord[i].sab_sex=='F')
            printf("Sex:female\n");
        else
            printf("Sex:invalid data entered\n");

        printf("Salary:Rs.%f\n",sab_pEmployeeRecord[i].sab_salary);

        if(sab_pEmployeeRecord[i].sab_marital_status=='Y')
            printf("Marital status: married\n");
        else if(sab_pEmployeeRecord[i].sab_marital_status=='N')
            printf("Marital status: unmarried\n");
        else
            printf("Marital status: invalid data entered\n");

        printf("\n\n");
    }

    if(sab_pEmployeeRecord)
    {
        free(sab_pEmployeeRecord);
        sab_pEmployeeRecord=NULL;
        printf("Memory allocated to %d emplyees freed!!!\n\n",sab_num_employees);
    }
    return 0;
}

void sab_MyGetString(char sab_str[],int sab_str_size)
{
    // Variable Declarations
    int i;
    char sab_ch='\0';
    // Code
    i = 0;
    do
    {
        sab_ch=getch();
        sab_str[i]=sab_ch;
        printf("%c",sab_str[i]);
        i++;
    }while ((sab_ch!='\r')&&(i<sab_str_size));

    if (i==sab_str_size)
        sab_str[i-1]='\0';
    else
        sab_str[i]='\0';
        
}
