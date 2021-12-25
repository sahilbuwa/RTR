#include <stdio.h>
struct sab_MyNumber
{
    int sab_num;
    int sab_num_table[10];
};
struct sab_NumTables
{
    struct sab_MyNumber a;
    struct sab_MyNumber b;
    struct sab_MyNumber c;
};

int main(int argc,char *argv[],char *envp[])
{
    //variable declarations
    struct sab_NumTables sab_tables;
    int i;
    //code
    sab_tables.sab_a.sab_num=2;
    for(i = 0;i < 10;i++)
        sab_tables.sab_a.sab_num_table[i]=sab_tables.sab_a.sab_num*(i+1);

    printf("\n\n");
    printf("Table 0f %d: \n\n",sab_tables.sab_a.sab_num);
    for(i = 0;i < 10;i++)
        printf("%d * %d= %d\n",sab_tables.sab_a.sab_num,(i+1),sab_tables.sab_a.sab_num_table[i]);

    sab_tables.sab_b.sab_num=3;
    for(i = 0;i < 10;i++)
        sab_tables.sab_b.sab_num_table[i]=sab_tables.sab_b.sab_num*(i+1);

    printf("\n\n");
    printf("Table 0f %d:\n\n",sab_tables.sab_b.sab_num);
    for(i= 0;i < 10;i++)
        printf("%d * %d = %d\n", tables.b.num, (i + 1), tables.b.num_table[i]);

    sab_tables.sab_c.sab_num=4;
    for(i = 0;i < 10;i++)
        sab_tables.sab_c.sab_num_table[i]=sab_tables.sab_c.sab_num*(i+1);

    printf("\n\n");
    printf("Table 0f %d:\n\n",sab_tables.sab_c.sab_num);
    for(i = 0;i < 10;i++)
        printf("%d *%d= %d\n",sab_tables.sab_c.sab_num,(i+1),sab_tables.sab_c.sab_num_table[i]);

    return 0;
}
