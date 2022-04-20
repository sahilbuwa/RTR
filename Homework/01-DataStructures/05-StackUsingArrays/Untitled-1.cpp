#include<stdio.h>

#define MATRIX_SIZE

struct stacknode
{
    float matrix[MATRIX_SIZE];
    struct stacknode* next;
};

typedef struct stacknode STACK;

STACK* top=NULL;

int main(void)
{
    // Function Declarations
    
}