#include <stdio.h>
int main(void)
{
    //variable declarations
    char sab_chArray_01[] = { 'A', 'S', 'T', 'R', 'O', 'M', 'E', 'D', 'I', 'C', 'O', 'M', 'P', '\0' };
    char sab_chArray_02[9] = { 'W', 'E', 'L', 'C', 'O', 'M', 'E', 'S', '\0' }; 
    char sab_chArray_03[] = { 'Y', 'O', 'U', '\0' };
    char sab_chArray_04[] = "To"; 
    char sab_chArray_05[] = "REAL TIME RENDERING BATCH OF 2020-21";
    char sab_chArray_WithoutNullTerminator[] = { 'H', 'e', 'l', 'l', 'o' };
    //code
    printf("\n\n");
    printf("Size Of chArray_01 : %lu\n\n", sizeof(sab_chArray_01));
    printf("Size Of chArray_02 : %lu\n\n", sizeof(sab_chArray_02));
    printf("Size Of chArray_03 : %lu\n\n", sizeof(sab_chArray_03));
    printf("Size Of chArray_04 : %lu\n\n", sizeof(sab_chArray_04));
    printf("Size Of chArray_05 : %lu\n\n", sizeof(sab_chArray_05));

    printf("\n\n");

    printf("The Strings Are : \n\n");
    printf("chArray_01 : %s\n\n", sab_chArray_01);
    printf("chArray_02 : %s\n\n", sab_chArray_02);
    printf("chArray_03 : %s\n\n", sab_chArray_03);
    printf("chArray_04 : %s\n\n", sab_chArray_04);
    printf("chArray_05 : %s\n\n", sab_chArray_05);

    printf("\n\n");
    
    printf("Size Of chArray_WithoutNullTerminator : %lu\n\n", sizeof(sab_chArray_WithoutNullTerminator));
    printf("chArray_WithoutNullTerminator : %s\n\n", sab_chArray_WithoutNullTerminator); 
    return 0;
}
