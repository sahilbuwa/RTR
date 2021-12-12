#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
    //function prototypes
    void sab_display_information(void);
    void sab_Function_Country(void);
    //code
    sab_display_information(); 
    sab_Function_Country(); 
    return(0);
}
    
void sab_display_information(void) 
{
    //function prototypes
    void sab_Function_My(void);
    void sab_Function_Name(void);
    void sab_Function_Is(void);
    void sab_Function_FirstName(void);
    void sab_Function_MiddleName(void);
    void sab_Function_Surname(void);
    void sab_Function_OfAMC(void);
    //code
    sab_Function_My();
    sab_Function_Name();
    sab_Function_Is();
    sab_Function_FirstName();
    sab_Function_MiddleName();
    sab_Function_Surname();
    sab_Function_OfAMC();
}
void sab_Function_My(void) //function definition
{
    //code
    printf("\n\n");
    printf("My");
}
void sab_Function_Name(void) //function definition
{
    //code
    printf("\n\n");
    printf("Name");
}
void sab_Function_Is(void) //function definition
{
    //code
    printf("\n\n");
    printf("Is");
}
void sab_Function_FirstName(void) //function definition
{
    printf("\n\n");
    printf("Sahil");
}
void sab_Function_MiddleName(void) //function definition
{
    printf("\n\n");
    printf("Ajit");
}
void sab_Function_Surname(void) //function definition
{
    printf("\n\n");
    printf("Buwa");
}
void sab_Function_OfAMC(void) //function definition
{
    printf("\n\n");
    printf("Of ASTROMEDICOMP");
}
void sab_Function_Country(void) //function definition
{
    printf("\n\n");
    printf("I live In India.");
    printf("\n\n");
}
