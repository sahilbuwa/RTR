#include<stdio.h>
#include<stdlib.h>

#define MAX_STRING_LENGTH 512

int main(int argc,char *argv[],char *envp[])
{
    // Function signature
    int sab_MyStrlen(char *);

    // Variable declarations
    char *sab_chArray = NULL;
    int sab_iStringLength=0;

    // Code
    printf("\n\n");
    sab_chArray=(char *)malloc(MAX_STRING_LENGTH*sizeof(char));
    if(sab_chArray==NULL)
    {
        printf("Memory allocation failed!!Exiting now...\n\n");
        exit(0);
    }

    printf("Enter a string:\n\n");
    gets_s(sab_chArray,MAX_STRING_LENGTH);

    printf("\n\n");
    printf("String entered by you is : \n\n");
    printf("%s\n",sab_chArray);

    printf("\n\n");
    sab_iStringLength=sab_MyStrlen(sab_chArray);
    printf("Length of string is =%d characters !!\n\n",sab_iStringLength);

    if(sab_chArray)
    {
        free(sab_chArray);
        sab_chArray=NULL;
    }
    return 0;
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
