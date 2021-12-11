#include<stdio.h>

int main(int sab_argc,char *sab_argv[],char *sab_envp[])
{
    //code
    printf("\n\n");
    printf("Hello World.\n");
    printf("Number of command line arguments = %d\n\n",sab_argc);

    printf("Command line arguments passed to this program are : \n\n");
    for(int i=0;i < sab_argc;i++)
    {
        printf("Command line argument number is  %d=%s\n",(i+1),sab_argv[i]);
    }
    printf("\n\n");

    printf("First 20 environment variables passed to this program are : \n\n");
    for(int i=0;i<20;i++)
    {
        printf("Environmental variable number %d = %s \n",(i+1),sab_envp[i]);

    }
    printf("\n\n");
    return 0;
}