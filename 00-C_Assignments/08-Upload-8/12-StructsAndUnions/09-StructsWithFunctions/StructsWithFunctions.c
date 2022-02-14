#include<stdio.h>

// Struct definitions
struct sab_MyData
{
    int sab_i;
    float sab_f;
    double sab_d;
    char sab_c;
};

int main(int argc,char *argv[],char *envp[])
{
    // Function declaration
    struct sab_MyData sab_AddStructMembers(struct sab_MyData,struct sab_MyData,struct sab_MyData);

    // Variable declaration
    struct sab_MyData sab_data1,sab_data2,sab_data3,sab_answer_data;

    // code
    // Data1 entry
    printf("\n\n\n\n\n");
    printf("********************* Data 1 ********************\n\n");
    printf("Enter Integer Value for i of struct Mydata data1 : ");
    scanf("%d",&sab_data1.sab_i);

    printf("\n\n");
    printf("Enter floating point value for f of struct MyData data1 : ");
    scanf("%f",&sab_data1.sab_f);

    printf("\n\n");
    printf("Enter double point value for d of struct MyData data1 : ");
    scanf("%lf",&sab_data1.sab_d);

    printf("\n\n");
    printf("Enter character point value for c of struct MyData data1 : ");
    sab_data1.sab_c = getch();
    printf("%c",sab_data1.sab_c);

    // Data 2 entry
    printf("\n\n\n\n\n");
    printf("*********************** Data 2 *****************\n\n");
    printf("Enter integer value for i of struct MyData data2 : ");
    scanf("%d",&sab_data2.sab_i);

    printf("\n\n");
    printf("Enter floating-point value for 'f' of struct MyData data2 : ");
    scanf("%f",&sab_data2.sab_f);

    printf("\n\n");
    printf("Enter floating-point value for 'd' of struct MyData data2 : ");
    scanf("%lf",&sab_data2.sab_d);

    printf("\n\n");
    printf("Enter floating-point value for 'c' of struct MyData data2 : ");
    sab_data2.sab_c = getch();
    printf("%c",sab_data2.sab_c);

    // Data 3 entry
    printf("\n\n\n\n");
    printf("****************** Data 3 *******************\n\n");
    printf("Enter Integer value for 'i' of struct MyData data3 : ");
    scanf("%d",&sab_data3.sab_i);

    printf("\n\n");
    printf("Enter floating point value for 'f' of struct MyData data3 : ");
    scanf("%f",&sab_data3.sab_f);

    printf("\n\n");
    printf("Enter double point value for 'd' of struct MyData data3 : ");
    scanf("%lf",&sab_data3.sab_d);
    
    printf("\n\n");
    printf("Enter character point value for 'c' of struct MyData data3 : ");
    sab_data3.sab_c = getch();
    printf("%c",sab_data3.sab_c);


    sab_answer_data = sab_AddStructMembers(sab_data1,sab_data2,sab_data3);

    printf("\n\n\n\n");
    printf("******** Answer *******\n\n");
    printf("answer_data.i = %d\n",sab_answer_data.sab_i);
    printf("answer_data.f = %f\n",sab_answer_data.sab_f);
    printf("answer_data.d = %lf\n\n",sab_answer_data.sab_d);


    sab_answer_data.sab_c = sab_data1.sab_c;
    printf("answer_data.c(from data1) = %c\n\n",sab_answer_data.sab_c);
    sab_answer_data.sab_c = sab_data2.sab_c;
    printf("answer_data.c(from data2) = %c\n\n",sab_answer_data.sab_c);
    sab_answer_data.sab_c = sab_data3.sab_c;
    printf("answer_data.c(from data3) = %c\n\n",sab_answer_data.sab_c);

    return 0;

}

struct sab_MyData sab_AddStructMembers(struct sab_MyData sab_md_one, struct sab_MyData sab_md_two, struct sab_MyData sab_md_three)
{
    // variable declarations
    struct sab_MyData sab_answer;

    // code
    sab_answer.sab_i = sab_md_one.sab_i+sab_md_two.sab_i+sab_md_three.sab_i;
    sab_answer.sab_f = sab_md_one.sab_f+sab_md_two.sab_f+sab_md_three.sab_f;
    sab_answer.sab_d = sab_md_one.sab_d+sab_md_two.sab_d+sab_md_three.sab_d;
    return sab_answer;
}
