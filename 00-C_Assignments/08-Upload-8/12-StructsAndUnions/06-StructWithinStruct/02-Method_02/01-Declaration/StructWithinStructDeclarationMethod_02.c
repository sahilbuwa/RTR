#include <stdio.h>

//Struct Declarations
struct sab_MyPoint
{
    int sab_x;
    int sab_y;
};

struct sab_Rectangle
{
    struct sab_MyPoint sab_point_01,sab_point_02;
};

int main(int argc,char *argv[],char *envp[])
{
    //variable declarations
    int sab_length,sab_breadth,sab_area;
    struct sab_Rectangle sab_rect;
    //code
    printf("\n\n");
    printf("Enter leftmost X-Coordinate of rectangle: ");
    scanf("%d",&sab_rect.sab_point_01.sab_x);
    printf("\n\n");
    printf("Enter bottommost Y-Coordinate of rectangle: ");
    scanf("%d",&sab_rect.sab_point_01.sab_y);
    printf("\n\n");
    printf("Enter rightmost X-Coordinate of rectangle: ");
    scanf("%d",&sab_rect.sab_point_02.sab_x);
    printf("\n\n");
    printf("Enter topmost Y-Coordinate of rectangle: ");
    scanf("%d",&sab_rect.sab_point_02.sab_y);

    sab_length=sab_rect.sab_point_02.sab_y-sab_rect.sab_point_01.sab_y;
    if(sab_length < 0)
        sab_length=sab_length*-1;

    sab_breadth=sab_rect.sab_point_02.sab_x -sab_rect.sab_point_01.sab_x;
    if(sab_breadth < 0)
        sab_breadth=sab_breadth*-1;

    sab_area=sab_length*sab_breadth;
    
    printf("\n\n");
    printf("Length of rectangle =%d\n\n",sab_length);
    printf("Breadth of rectangle=%d\n\n",sab_breadth);
    printf("Area 0f rectangle   =%d\n\n",sab_area);

    return 0;
}
