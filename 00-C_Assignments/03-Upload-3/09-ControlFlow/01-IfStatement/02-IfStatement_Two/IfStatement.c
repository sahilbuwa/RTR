#include<stdio.h>
int main(void)
{
    //variable declarations
    int sab_age;

    //code
    printf("\n\n");
    printf("Enter Age : ");
    scanf("%d", &sab_age);
    if(sab_age >= 18)
    {
        printf("You are eligible for voting !!!\n\n");
    }
    return 0;
}