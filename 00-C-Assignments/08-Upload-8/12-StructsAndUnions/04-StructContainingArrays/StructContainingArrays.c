#include<stdio.h>

#define INT_ARRAY_SIZE 10
#define FLOAT_ARRAY_SIZE 5
#define CHAR_ARRAY_SIZE 26

#define NUM_STRINGS 10
#define MAX_CHARACTERS_PER_STRING 20

#define ALPHABET_BEGINNING 65 

//Struct Declarations
struct sab_MyDataOne
{
    int sab_iArray[INT_ARRAY_SIZE];
    float sab_fArray[FLOAT_ARRAY_SIZE];
};

struct sab_MyDataTwo
{
    char sab_cArray[CHAR_ARRAY_SIZE];
    char sab_strArray[NUM_STRINGS][MAX_CHARACTERS_PER_STRING];
};

int main(int argc,char* argv[],char* envp[])
{
    //variable declarations
    struct sab_MyDataOne sab_data_one;
    struct sab_MyDataTwo sab_data_two;
    int sab_i;

    //code
    //Assign values
    sab_data_one.sab_fArray[0] = 0.1f;
    sab_data_one.sab_fArray[1] = 1.2f;
    sab_data_one.sab_fArray[2] = 2.3f;
    sab_data_one.sab_fArray[3] = 3.4f;
    sab_data_one.sab_fArray[4] = 4.5f;

    //User input
    printf("\n\n");
    printf("Enter %d integers : \n\n", INT_ARRAY_SIZE);
    for (sab_i=0;sab_i < INT_ARRAY_SIZE;sab_i++)
        scanf("%d", &sab_data_one.sab_iArray[sab_i]);

    for (sab_i=0;sab_i < CHAR_ARRAY_SIZE;sab_i++)
        sab_data_two.sab_cArray[sab_i] = (char)(sab_i + ALPHABET_BEGINNING);

    strcpy(sab_data_two.sab_strArray[0],"Welcome !!!");
    strcpy(sab_data_two.sab_strArray[1],"This");
    strcpy(sab_data_two.sab_strArray[2],"Is");
    strcpy(sab_data_two.sab_strArray[3],"Astromedicomp'S");
    strcpy(sab_data_two.sab_strArray[4],"Real");
    strcpy(sab_data_two.sab_strArray[5],"Time");
    strcpy(sab_data_two.sab_strArray[6],"Rendering");
    strcpy(sab_data_two.sab_strArray[7],"Batch");
    strcpy(sab_data_two.sab_strArray[8],"Of");
    strcpy(sab_data_two.sab_strArray[9],"2021-2022 !!!");

    //Display values struct1
    printf("\n\n");
    printf("Members of 'struct DataOne' along with their assigned values are : \n\n");

    printf("\n\n");
    printf("Integer array (data_one.iArray[]) : \n\n");
    for(sab_i=0;sab_i < INT_ARRAY_SIZE;sab_i++)
        printf("data_one.iArray[%d] = %d\n", sab_i, sab_data_one.sab_fArray[sab_i]);

    printf("\n\n");
    printf("Floating-point array (data_one.fArray[]) : \n\n");
    for(sab_i = 0;sab_i < FLOAT_ARRAY_SIZE;sab_i++)
        printf("data_one.fArray[%d] = %f\n", sab_i,sab_data_one.sab_fArray[sab_i]);

    //Display values struct2
    printf("\n\n");
    printf("Members of 'struct DataTwo' along with their assigned values are : \n\n");

    printf("\n\n");
    printf("Character array (data_two.cArray[]) : \n\n");
    for (sab_i = 0; sab_i < CHAR_ARRAY_SIZE; sab_i++)
        printf("data_two.cArray[%d] = %c\n", sab_i, sab_data_two.sab_cArray[sab_i]);

    printf("\n\n");
    printf("String array (data_two.strArray[]) : \n\n");
    for (sab_i = 0;sab_i < NUM_STRINGS;sab_i++)
        printf("%s",sab_data_two.sab_strArray[sab_i]);

    printf("\n\n");

    return 0;

}
