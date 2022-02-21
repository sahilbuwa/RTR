#include<stdio.h>
#include<string.h>

int main(int argc,char *argv[],char *envp[])
{
    // Variable Declarations
    char sab_ch,sab_ch_i;
    unsigned int sab_ascii_ch=0;

    // Code

    printf("\n\n");
    printf("Enter the first char of first name : \n");
    sab_ch=getch();
    sab_ch=toupper(sab_ch);

    for(sab_ch_i='A';sab_ch<='Z';sab_ch_i++)
    {
        if(sab_ch==sab_ch_i)
        {
            sab_ascii_ch=(unsigned int)sab_ch;
            goto sab_result_output;
        }
    }
    printf("\n\n");
    printf("Goto statement not executed so printing \"Hello,World!!!\",\n");

sab_result_output:
    printf("\n\n");

    if(sab_ascii_ch==0)
    {
        printf("Could not find the character %c in the alphabet",sab_ch);
    }
    else
        printf("Character %c found.It has ASCII value %u\n",sab_ch,sab_ascii_ch);

    printf("\n\n");
    return 0;
}
