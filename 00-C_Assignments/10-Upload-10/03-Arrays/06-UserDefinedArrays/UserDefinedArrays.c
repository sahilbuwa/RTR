#include <stdio.h>
#include <stdlib.h>
#define INT_SIZE sizeof(int)
#define FLOAT_SIZE sizeof(float)
#define DOUBLE_SIZE sizeof(double)
#define CHAR_SIZE sizeof(char)

int main(int argc,char* argv[],char *envp[])
{
    // Variable declarations
    int *sab_ptr_iArray = NULL,i;
    unsigned int sab_intArrayLength = 0;

    float *sab_ptr_fArray = NULL;
    unsigned int sab_floatArrayLength = 0;

    double *sab_ptr_dArray = NULL;
    unsigned int sab_doubleArrayLength = 0;

    char *sab_ptr_cArray = NULL;
    unsigned int sab_charArrayLength = 0;

    //code
    //******Int*********
    printf("\n\n");
    printf("Enter the number of elements you want in the integer array : ");
    scanf("%u",&sab_intArrayLength);
    sab_ptr_iArray=(int *)malloc(INT_SIZE*sab_intArrayLength);
    if (sab_ptr_iArray == NULL)
    {
        printf("\n\n");
        printf("Memory allocation for integer array failed !!Exiting now...\n\n");
        exit(0);
    }
    else
    {
        printf("\n\n");
        printf("Memory allocation for integer array succeeded !!\n\n");
    }
    printf("\n\n");

    printf("Enter the %d integer elements to fill up the integer array : \n\n",sab_intArrayLength);
    for (i=0;i<sab_intArrayLength;i++)
        scanf("%d",(sab_ptr_iArray + i));

    // ****** Float *******
    printf("\n\n");

    printf("Enter the number of elements you want in the 'float' array : ");
    scanf("%u",&sab_floatArrayLength);
    sab_ptr_fArray = (float *)malloc(FLOAT_SIZE*sab_floatArrayLength);

    if (sab_ptr_fArray == NULL)
    {
        printf("\n\n");
        printf("Memory allocation for floating array failed !!Exiting now...\n\n");
        exit(0);
    }

    else
    {
        printf("\n\n");
        printf("Memory allocation for floating array succeeded !!!\n\n");
    }

    printf("\n\n");
    printf("Enter the %d floating-point elements to fill up the 'float' array : \n\n",sab_floatArrayLength);
    for(i=0;i<sab_floatArrayLength;i++)
        scanf("%f",(sab_ptr_fArray + i));

    // ******Double******
    printf("\n\n");
    printf("Enter the number of elements you want in the 'double' array: ");
    scanf("%u",&sab_doubleArrayLength);
    sab_ptr_dArray=(double *)malloc(DOUBLE_SIZE*sab_doubleArrayLength);
    if(sab_ptr_dArray == NULL)
    {
        printf("\n\n");
        printf("Memory allocation for double array failed !!Exiting now...\n\n");
        exit(0);
    }
    else
    {
        printf("\n\n");
        printf("Memory allocation for double array succeeded !!\n\n");
    }
    printf("\n\n");
    printf("Enter the %d double elements to fill up the 'double' array : \n\n",sab_doubleArrayLength);
    for (i=0;i<sab_doubleArrayLength;i++)
    scanf("%lf",(sab_ptr_dArray+i));

    // ******Char*****
    printf("\n\n");
    printf("Enter the number of elements you want in the character array: ");
    scanf("%u",&sab_charArrayLength);
    sab_ptr_cArray=(char *)malloc(CHAR_SIZE*sab_charArrayLength);
    if (sab_ptr_cArray == NULL)
    {
        printf("\n\n");
        printf("Memory allocation for char array failed !!Exiting now...\n\n");
        exit(0);
    }
    else
    {
        printf("\n\n");
        printf("Memory allocation for char array succeeded !!\n\n");
    }
    printf("\n\n");
    printf("Enter the %d character elements to fill up the character array :\n\n",sab_charArrayLength);
    for (i=0;i<sab_charArrayLength;i++)
    {
        *(sab_ptr_cArray+i) = getch();
        printf("%c\n",*(sab_ptr_cArray+i));
    }
    // Display
    printf("\n\n");
    printf("The integer array entered by you and consisting of %d elements is as follows : \n\n",sab_intArrayLength);
    for (i=0;i<sab_intArrayLength;i++)
        printf(" %d \t \t at address : %p\n",*(sab_ptr_iArray+i),(sab_ptr_iArray+i));


    printf("\n\n");
    printf("The float array entered by you and consisting of %d elements is as follows : \n\n",sab_floatArrayLength);
    for (i=0;i<sab_floatArrayLength;i++)
        printf(" %f \t \t at address : %p\n",*(sab_ptr_fArray+i),(sab_ptr_fArray+i));


    printf("\n\n");
    printf("The double array entered by you and consisting Of %d elements is as follows : \n\n",sab_doubleArrayLength);
    for (i=0;i<sab_doubleArrayLength;i++)
    printf(" %lf \t \t At Address : %p\n",*(sab_ptr_dArray+i),(sab_ptr_dArray+i));
    

    printf("\n\n");
    printf("The character array entered by you and consisting of %d elements is as follows : \n\n",sab_charArrayLength);
    for (i=0;i<sab_charArrayLength;i++)
        printf(" %c \t \t at address : %p\n",*(sab_ptr_cArray+i),(sab_ptr_cArray+i));
    
    if(sab_ptr_cArray)
    {
        free(sab_ptr_cArray);
        sab_ptr_cArray = NULL;
        printf("\n\n");
        printf("Memory occupied by character array has been successfully freed!!!\n\n");
    }
    if(sab_ptr_fArray)
    {
        free(sab_ptr_fArray);
        sab_ptr_fArray = NULL;
        printf("\n\n");
        printf("Memory occupied by float array has been successfully freed!!!\n\n");
    }
    if(sab_ptr_dArray)
    {
        free(sab_ptr_dArray);
        sab_ptr_dArray = NULL;
        printf("\n\n");
        printf("Memory occupied by double array has been successfully freed!!!\n\n");
    }
    if(sab_ptr_iArray)
    {
        free(sab_ptr_iArray);
        sab_ptr_iArray = NULL;
        printf("\n\n");
        printf("Memory occupied by integer array has been successfully freed!!!\n\n");
    }
    return 0;
}
