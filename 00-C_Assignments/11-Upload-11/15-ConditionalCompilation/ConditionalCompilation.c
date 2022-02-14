#include<stdio.h>

//#define SAB_MY_PI_VERSION 10

#ifdef SAB_MY_PI_VERSION
#if SAB_MY_PI_VERSION <= 0
    #define SAB_MY_PI 3.14
#elif SAB_MY_PI_VERSION <= 1
    #define SAB_MY_PI 3.1415
#elif SAB_MY_PI_VERSION <= 2
    #define SAB_MY_PI 3.141592
#elif SAB_MY_PI_VERSION <= 3
    #define SAB_MY_PI 3.14159265
#elif SAB_MY_PI_VERSION <= 4
    #define SAB_MY_PI 3.1415926535
#elif SAB_MY_PI_VERSION <= 5
    #define SAB_MY_PI 3.141592653589
#else
    #define SAB_MY_PI 3.141592653589793
#endif
#endif

#ifndef SAB_MY_PI_VERSION
    #define SAB_MY_PI 0.0
#endif

int main(int argc,char *argv[],char *envp[])
{
    // Code
    printf("\n\n");
    printf("Hello World\n");
    printf("\n\n");
    //printf("PI version selected = %d\n",SAB_MY_PI_VERSION);
    printf("PI value = %.15lf\n",SAB_MY_PI);
    return 0;
}
