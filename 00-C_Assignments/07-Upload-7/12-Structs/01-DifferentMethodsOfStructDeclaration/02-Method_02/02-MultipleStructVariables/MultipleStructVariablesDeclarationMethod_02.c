#include<stdio.h>

struct sab_MyPoint
{
    int sab_x;
    int sab_y;
};

struct sab_MyPoint sab_point_A,sab_point_B,sab_point_C,sab_point_D,sab_point_E;

int main(int argc,char* argv[],char* envp[])
{
    //Code
    //Assign values A
    sab_point_A.sab_x = 3;
    sab_point_A.sab_y = 0;

    //Assign values B
    sab_point_B.sab_x = 1;
    sab_point_B.sab_y = 2;

    //Assign values C
    sab_point_C.sab_x = 9;
    sab_point_C.sab_y = 6;

    //Assign values D
    sab_point_D.sab_x = 8;
    sab_point_D.sab_y = 2;

    //Assign values E
    sab_point_E.sab_x = 11;
    sab_point_E.sab_y = 8;

    //Display values
    printf("\n\n");
    printf("Co-ordinates (x,y) of point 'A' are : (%d,%d)\n\n",sab_point_A.sab_x,sab_point_A.sab_y);
    printf("Co-ordinates (x,y) of point 'B' are : (%d,%d)\n\n",sab_point_B.sab_x,sab_point_B.sab_y);
    printf("Co-ordinates (x,y) of point 'C' are : (%d,%d)\n\n",sab_point_C.sab_x,sab_point_C.sab_y);
    printf("Co-ordinates (x,y) of point 'D' are : (%d,%d)\n\n",sab_point_D.sab_x,sab_point_D.sab_y);
    printf("Co-ordinates (x,y) of point 'E' are : (%d,%d)\n\n",sab_point_E.sab_x,sab_point_E.sab_y);

    return 0;

}
