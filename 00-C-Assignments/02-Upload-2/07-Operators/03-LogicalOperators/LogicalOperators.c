#include<stdio.h>

int main(void)
{
    //Variable Declarations
    int sab_a;
    int sab_b;
    int sab_c;
    int sab_result;

    //code
    printf("\n\n");
    printf("Enter First Integer : ");
    scanf("%d",&sab_a);

    printf("\n\n");
    printf("Enter Second Integer : ");
    scanf("%d",&sab_b);

    printf("\n\n");
    printf("Enter Third Integer : ");
    scanf("%d",&sab_c);

    printf("\n\n");
    printf("If Answer = 0, It Is 'False'.\n");
    printf("If Answer = 1, It Is 'True'.\n");

    sab_result=(sab_a<=sab_b)&&(sab_b!=sab_c);
    printf("LOGICAL AND (&&) : Answer is TRUE (1) If And Only If BOTH conditions are true. The answer is FALSE (0), If any one or both conditions are false.\n\n");
    printf("A= %d is less than or equal to B = %d is not equal to C = %d    \t Answer = %d\n\n",sab_a,sab_b,sab_b,sab_c,sab_result);

    sab_result=(sab_b>=sab_a)||(sab_a==sab_c);
    printf("LOGICAL OR (||) : Answer is False (0) If And Only If BOTH conditions are False. The answer is True (1), If any one or both conditions are True.\n\n");
    printf("Either B= %d is greater than or equal to A = %d is equal to C = %d    \t Answer = %d\n\n",sab_b,sab_a,sab_a,sab_c,sab_result);
    
    sab_result= !sab_a;
    printf("A = %d and using logical NOT (!) operator on A gives result = %d\n\n",sab_a,sab_result);

    sab_result= !sab_b;
    printf("B = %d and using logical NOT (!) operator on B gives result = %d\n\n",sab_b,sab_result);
    
    sab_result= !sab_c;
    printf("C = %d and using logical NOT (!) operator on C gives result = %d\n\n",sab_c,sab_result);
    
    sab_result = (!(sab_a <= sab_b) && !(sab_b != sab_c));
    printf("Using logical NOT (!) on (a <=b) and also on (b != c) and then ANDing them afterwards gives result = %d\n",sab_result);

    printf("\n\n");

    sab_result = !((sab_b >= sab_a) || (sab_a == sab_c));
    printf("Using logical NOT (!) on entire logical expression (b >= a) || (a==c) gives result = %d\n",sab_result);

    printf("\n\n");

    return 0;    
}
