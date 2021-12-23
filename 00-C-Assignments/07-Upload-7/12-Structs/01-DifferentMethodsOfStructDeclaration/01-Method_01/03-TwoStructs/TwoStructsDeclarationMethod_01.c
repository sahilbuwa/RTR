#include<stdio.h>

//Defining Struct here
struct sab_MyPoint
{
    int sab_x;
    int sab_y;
} sab_point;

//Defining another Struct
struct sab_MyPointProperties
{
    int sab_quadrant;
    char sab_axis_location[10];
} sab_point_properties;

int main(int argc,char *argv[],char *envp[])
{
    //code
    //User input
    printf("\n\n");
    printf("Enter X-Coordinate for A point:");
    scanf("%d",&sab_point.sab_x);
    printf("Enter Y-Coordinate for A point:");
    scanf("%d",&sab_point.sab_y);

    printf("\n\n");
    printf("Point Co-ordinates (x,y) are : (%d,%d)!!!\n",sab_point.sab_x,sab_point.sab_y);

    if(sab_point.sab_x == 0 && sab_point.sab_y == 0) //Origin
        printf("The point is the origin (%d,%d) !!!\n",sab_point.sab_x,sab_point.sab_y);
    else
    {
        if(sab_point.sab_x == 0)//Yaxis 
        {
            if(sab_point.sab_y < 0) 
                strcpy(sab_point_properties.sab_axis_location, "Negative Y");

            if(sab_point.sab_y > 0)
                strcpy(sab_point_properties.sab_axis_location, "Positive Y");

            sab_point_properties.sab_quadrant = 0;
            printf("The point lies on the %s axis!!!\n\n",sab_point_properties.sab_axis_location);
        }
        else if(sab_point.sab_y == 0)//Xaxis
        {
            if(sab_point.sab_x < 0)
                strcpy(sab_point_properties.sab_axis_location, "Negative X");

            if(sab_point.sab_x > 0)
                strcpy(sab_point_properties.sab_axis_location, "Positive X");

            sab_point_properties.sab_quadrant = 0;
            printf("The point lies on the %s axis!!!\n\n",sab_point_properties.sab_axis_location);
        }
        else //Kuthlatari quadrant
        {
            sab_point_properties.sab_axis_location[0] = '\0';

            if(sab_point.sab_x > 0 && sab_point.sab_y > 0)
                sab_point_properties.sab_quadrant = 1;

            else if(sab_point.sab_x < 0 && sab_point.sab_y > 0) 
                sab_point_properties.sab_quadrant = 2;

            else if(sab_point.sab_x < 0 && sab_point.sab_y < 0) 
                sab_point_properties.sab_quadrant = 3;
            else 
                sab_point_properties.sab_quadrant = 4;

            printf("The point lies in quadrant number %d!!!\n\n",sab_point_properties.sab_quadrant);
        }
    }
    return 0;
}
