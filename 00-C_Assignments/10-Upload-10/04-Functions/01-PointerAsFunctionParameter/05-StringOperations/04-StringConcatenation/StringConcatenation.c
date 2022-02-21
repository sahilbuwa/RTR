#include<stdio.h>
#include<stdlib.h>

#define MAX_STRING_LENGTH 512

int main(int argc,char *argv[],char *envp[])
{
    // Function signature
    int sab_MyStrlen(char *);
    void sab_MyStrcat(char *,char *);

    // Variable declarations
    char *sab_chArray_One = NULL,*sab_chArray_Two=NULL;

    // Code
    printf("\n\n");
    sab_chArray_One=(char *)malloc(MAX_STRING_LENGTH*sizeof(char));
    if(sab_chArray_One==NULL)
    {
        printf("Memory allocation of 1st failed!!Exiting now...\n\n");
        exit(0);
    }

    printf("Enter a string:\n\n");
    gets_s(sab_chArray_One,MAX_STRING_LENGTH);

    printf("\n\n");
    sab_chArray_Two=(char *)malloc(MAX_STRING_LENGTH*sizeof(char));
    if(sab_chArray_Two==NULL)
    {
        printf("Memory allocatiwon of 2nd failed!!Exiting now...\n\n");
        exit(0);
    }

    printf("Enter second string :\n\n");
    gets_s(sab_chArray_Two,MAX_STRING_LENGTH);

    printf("**********Before Concatenation******");
    printf("\n\n");
    printf("1st String entered by you is : \n\n");
    printf("%s\n",sab_chArray_One);

    printf("\n\n");
    printf("The 2nd string is :\n\n");
    printf("%s\n",sab_chArray_Two);

    sab_MyStrcat(sab_chArray_One,sab_chArray_Two);

    printf("*******After Concatenation******");
    printf("\n\n");
    printf("1st String entered by you is : \n\n");
    printf("%s\n",sab_chArray_One);

    printf("\n\n");
    printf("The 2nd string is :\n\n");
    printf("%s\n",sab_chArray_Two);

    if(sab_chArray_Two)
    {
        free(sab_chArray_Two);
        sab_chArray_Two=NULL;
        printf("\n\n");
        printf("Memory allocated for 2nd string has been freed!!\n\n");
    }

    
    if(sab_chArray_One)
    {
        free(sab_chArray_One);
        sab_chArray_One=NULL;
        printf("\n\n");
        printf("Memory allocated for 1st string has been freed!!\n\n");
    }

    return 0;
}

void sab_MyStrcat(char *sab_str_destination,char *sab_str_source)
{
    // Function prototype
    int MyStrlen(char *);
    //variable declarations
    int sab_iStringLength_Source=0,sab_iStringLength_Destination=0,i,j;
    // Code
    sab_iStringLength_Source=sab_MyStrlen(sab_str_source);
    sab_iStringLength_Destination=sab_MyStrlen(sab_str_destination);
    
    for(i=sab_iStringLength_Destination,j=0;j<sab_iStringLength_Source;i++,j++)
    {
        *(sab_str_destination+i)=*(sab_str_source+j);
    }
    *(sab_str_destination+i)='\0';
}

int sab_MyStrlen(char *sab_str)
{
    // Variable Declarations
    int j,sab_string_length=0;

    // Code
    for(j=0;j<MAX_STRING_LENGTH;j++)
    {
        if(*(sab_str+j)=='\0')
            break;
        else
            sab_string_length++;
    }
    return sab_string_length;
}
