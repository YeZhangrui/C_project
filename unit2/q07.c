#include <stdio.h>
int main(void)
{   int amount;
    printf("Enter a dollar amount:");
    scanf("%d",&amount);
    int a20=amount/20;
    printf("$20 bills:  %d\n",a20);
    int a10=(amount-20*a20)/10;
    printf("$10 bills:  %d\n",a10);
    int a5=(amount-20*a20-10*a10)/5;
    printf(" $5 bills:  %d\n",a5);
    int a1=amount-20*a20-10*a10-5*a5;
    printf(" $1 bills:  %d\n",a1);
    return 0;


}