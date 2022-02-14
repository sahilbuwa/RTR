#include<stdio.h>
#include<stdlib.h>

#define MAX_STRING_LENGTH 512

int main(int argc,char *argv[],char *envp[])
{
    // Function signature
    int sab_MyStrlen(char *);
    void sab_MyStrrev(char *,char *);

    // Variable declarations
    char *sab_chArray_Original = NULL,*sab_chArray_Reversed=NULL;
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
    sab_chArray_Reversed=(char *)malloc(sab_original_string_length*sizeof(char));
    if(sab_chArray_Reversed==NULL)
    {
        printf("Memory allocation of reversed failed!!Exiting now...\n\n");
        exit(0);
    }

    sab_MyStrrev(sab_chArray_Reversed,sab_chArray_Original);


    printf("\n\n");
    printf("Original String entered by you is : \n\n");
    printf("%s\n",sab_chArray_Original);

    printf("\n\n");
    printf("The reversed string is :\n\n");
    printf("%s\n",sab_chArray_Reversed);

    if(sab_chArray_Reversed)
    {
        free(sab_chArray_Reversed);
        sab_chArray_Reversed=NULL;
        printf("\n\n");
        printf("Memory allocated for reversed string has been freed!!\n\n");
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

void sab_MyStrrev(char *sab_str_destination, char *sab_str_source)
{
    // Function prototype
    int sab_MyStrlen(char *);
    // Variable declarations
    int sab_iStringLength = 0,i,j,sab_len;
    // Code
    sab_iStringLength=sab_MyStrlen(sab_str_source);
   
    sab_len=sab_iStringLength-1;
    
    for(i=0,j=sab_len;i<sab_iStringLength,j>= 0;i++,j--)
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
