#include <stdio.h>
int main(void)
{
    int a;
    printf("Enter a three-digit number:");
    scanf("%d",&a);
    int third=a/100;
    int second=a%100/10;
    int first=a%100%10;
    printf("The reversal is:%d%d%d",first,second,third);
    return 0;
}