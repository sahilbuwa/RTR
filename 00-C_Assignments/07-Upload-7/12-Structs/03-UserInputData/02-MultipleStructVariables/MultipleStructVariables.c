#include<stdio.h>

struct sab_MyPoint
{
    int sab_x;
    int sab_y;
};

int main(int argc,char* argv[],char* envp[])
{
    //Variable Declarations
    struct sab_MyPoint sab_point_A,sab_point_B,sab_point_C,sab_point_D,sab_point_E;

    //Code
    //User input A
    printf("\n\n");
    printf("Enter X-Coordinate for point 'A': ");
    scanf("%d", &sab_point_A.sab_x);
    printf("Enter Y-Coordinate for point 'A': ");
    scanf("%d", &sab_point_A.sab_y);

    //User input B
    printf("\n\n");
    printf("Enter X-Coordinate for point 'B': ");
    scanf("%d", &sab_point_B.sab_x);
    printf("Enter Y-Coordinate for point 'B': ");
    scanf("%d", &sab_point_B.sab_y);

    //User input C
    printf("\n\n");
    printf("Enter X-Coordinate for point 'C': ");
    scanf("%d", &sab_point_C.sab_x);
    printf("Enter Y-Coordinate for point 'C': ");
    scanf("%d", &sab_point_C.sab_y);

    //User input D
    printf("\n\n");
    printf("Enter X-Coordinate for point 'D': ");
    scanf("%d", &sab_point_D.sab_x);
    printf("Enter Y-Coordinate for point 'D': ");
    scanf("%d", &sab_point_D.sab_y);

    //User input E
    printf("\n\n");
    printf("Enter X-Coordinate for point 'E': ");
    scanf("%d", &sab_point_E.sab_x);
    printf("Enter Y-Coordinate for point 'E': ");
    scanf("%d", &sab_point_E.sab_y);

    //Display values
    printf("\n\n");
    printf("Co-ordinates (x,y) of point 'A' are : (%d,%d)\n\n",sab_point_A.sab_x,sab_point_A.sab_y);
    printf("Co-ordinates (x,y) of point 'B' are : (%d,%d)\n\n",sab_point_B.sab_x,sab_point_B.sab_y);
    printf("Co-ordinates (x,y) of point 'C' are : (%d,%d)\n\n",sab_point_C.sab_x,sab_point_C.sab_y);
    printf("Co-ordinates (x,y) of point 'D' are : (%d,%d)\n\n",sab_point_D.sab_x,sab_point_D.sab_y);
    printf("Co-ordinates (x,y) of point 'E' are : (%d,%d)\n\n",sab_point_E.sab_x,sab_point_E.sab_y);

    return 0;

}
