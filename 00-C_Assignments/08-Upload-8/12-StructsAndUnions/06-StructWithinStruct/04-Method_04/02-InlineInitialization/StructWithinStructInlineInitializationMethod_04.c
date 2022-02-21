#include <stdio.h>

int main(int argc, char *argv[],char *envp[])
{
    //Struct declarations
    struct sab_MyPoint
    {
        int sab_x;
        int sab_y;
    };

    struct sab_Rectangle
    {
        struct sab_MyPoint sab_point_01,sab_point_02;
    };
    struct sab_Rectangle sab_rect={{2,3},{5,6}};
    //variable declarations
    int sab_length,sab_breadth,sab_area;
    //code
    sab_length=sab_rect.sab_point_02.sab_y-sab_rect.sab_point_01.sab_y;
    if(sab_length < 0)
        sab_length=sab_length*-1;
    sab_breadth=sab_rect.sab_point_02.sab_x-sab_rect.sab_point_01.sab_x;
    if(sab_breadth < 0)
        sab_breadth=sab_breadth*-1;
    sab_area=sab_length*sab_breadth;
    //Display Values
    printf("\n\n");
    printf("Length of rectangle = %d\n\n",sab_length);
    printf("Breadth of rectangle= %d\n\n",sab_breadth);
    printf("Area 0f rectangle   = %d\n\n",sab_area);
    return 0;

}
