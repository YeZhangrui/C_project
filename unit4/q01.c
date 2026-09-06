#include <stdio.h>
int main(void)
{
    int a;
    printf("Enter a two-digit number:");
    scanf("%d",&a);
    int first=a%10;
    int second=a/10;
    printf("The reversal is:%d%d",first,second);
    return 0;
}