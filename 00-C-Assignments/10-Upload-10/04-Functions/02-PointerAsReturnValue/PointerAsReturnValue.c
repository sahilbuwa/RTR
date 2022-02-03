#include <stdio.h>
#include <stdlib.h>

#define MAX_STRING_LENGTH 512

int main(int argc,char *argv[],char *envp[])
{
    // Function declarations
    char* sab_ReplaceVowelsWithHashSymbol(char *);
    // Variable declaration
    char sab_string[MAX_STRING_LENGTH];
    char *sab_replaced_string=NULL;

    // Code

    printf("\n\n");
    printf("Enter string: ");
    gets_s(sab_string,MAX_STRING_LENGTH);
    sab_replaced_string=sab_ReplaceVowelsWithHashSymbol(sab_string);
    
    if(sab_replaced_string == NULL)
    {
        printf("ReplaceVowelsWithHashSymbol() function has failed !!! Exitiing Now...\n\n");
        exit(0);
    }

    printf("\n\n");
    printf("Replaced string is : \n\n");
    printf("%s\n\n",sab_replaced_string);

    if(sab_replaced_string)
    {
        free(sab_replaced_string);
        sab_replaced_string = NULL;
    }
    return 0;
}

char* sab_ReplaceVowelsWithHashSymbol(char *sab_s)
{
    // Function prototype
    void sab_MyStrcpy(char *, char *);
    int sab_MyStrlen(char *);
    // Varibale declarations
    char *sab_new_string=NULL;
    int i;

    // Code

    sab_new_string=(char *)malloc(sab_MyStrlen(sab_s)*sizeof(char));

    if(sab_new_string==NULL)
    {
        printf("Could not allocate memory!!!\n\n");
        return NULL;
    }
    sab_MyStrcpy(sab_new_string,sab_s);

    for(i=0;i<sab_MyStrlen(sab_new_string);i++)
    {
        switch(sab_new_string[i])
        {
            case 'A':
            case 'a':
            case 'E':
            case 'e':
            case 'I':
            case 'i':
            case 'O':
            case 'o':
            case 'U':
            case 'u':
                sab_new_string[i]='#';
                break;
            default:
                break;
        }
    }
    return sab_new_string;
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
