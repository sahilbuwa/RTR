#include<stdio.h>
#include<stdlib.h>

#define MAX_STRING_LENGTH 512

int main(int argc,char *argv[],char *envp[])
{
    // Function signature
    int sab_MyStrlen(char *);
    void sab_MyStrcpy(char *,char *);

    // Variable declarations
    char *sab_chArray_Original = NULL,*sab_chArray_Copy=NULL;
    int sab_original_string_length=0;

    // Code
    printf("\n\n");
    sab_chArray_Original=(char *)malloc(MAX_STRING_LENGTH*sizeof(char));
    if(sab_chArray_Original==NULL)
    {
        printf("Memory allocation of original failed!!Exiting now...\n\n");
        exit(0);
    }

    printf("Enter a string:\n\n");
    gets_s(sab_chArray_Original,MAX_STRING_LENGTH);

    sab_original_string_length=sab_MyStrlen(sab_chArray_Original);
    sab_chArray_Copy=(char *)malloc(sab_original_string_length*sizeof(char));
    if(sab_chArray_Copy==NULL)
    {
        printf("Memory allocation of copy failed!!Exiting now...\n\n");
        exit(0);
    }

    sab_MyStrcpy(sab_chArray_Copy,sab_chArray_Original);


    printf("\n\n");
    printf("Original String entered by you is : \n\n");
    printf("%s\n",sab_chArray_Original);

    printf("\n\n");
    printf("The copied string is :\n\n");
    printf("%s\n",sab_chArray_Copy);

    if(sab_chArray_Copy)
    {
        free(sab_chArray_Copy);
        sab_chArray_Copy=NULL;
        printf("\n\n");
        printf("Memory allocated for copy string has been freed!!\n\n");
    }

    
    if(sab_chArray_Original)
    {
        free(sab_chArray_Original);
        sab_chArray_Original=NULL;
        printf("\n\n");
        printf("Memory allocated for original string has been freed!!\n\n");
    }

    return 0;
}

void sab_MyStrcpy(char *sab_str_destination, char *sab_str_source)
{
    // Function prototype
    int sab_MyStrlen(char *);
    // Variable declarations
    int sab_iStringLength=0,j;
    // Code
    sab_iStringLength = sab_MyStrlen(sab_str_source);
    for(j=0;j<sab_iStringLength;j++)
        *(sab_str_destination+j) =*(sab_str_source + j);

    *(sab_str_destination+j)='\0';
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
