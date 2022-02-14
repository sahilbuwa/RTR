#include <stdio.h>
//Struct Declarations
struct sab_MyNumber
{
    int sab_num;
    int sab_num_table[10];
};
struct sab_NumTables
{
    struct sab_MyNumber sab_n;
};

int main(int argc,char *argv[],char *envp[])
{
    //variable declarations
    struct sab_NumTables sab_tables[10];
    int i, j;
    //code
    for(i = 0;i < 10;i++)
    {
        sab_tables[i].sab_n.sab_num=(i+1);
    }
    for(i = 0;i < 10;i++)
    {
        printf("\n\n");
        printf("Table of %d:\n\n",sab_tables[i].sab_n.sab_num);
        for(j = 0;j < 10;j++)
        {
            sab_tables[i].sab_n.sab_num_table[j]=sab_tables[i].sab_n.sab_num*(j+1);
            printf("%d * %d=%d\n",sab_tables[i].sab_n.sab_num,(j+1),sab_tables[i].sab_n.sab_num_table[j]);
        }
    }
    return 0;
}
