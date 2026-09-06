#include<stdio.h>
int main(void)
{
    int a,b;
    printf("Enter a number:");
    scanf("%d",&a);

    if (a<9)
        b=1;
    else if(a>9&&a<99)
        b=2;
    else if(a>99&&a<999)
        b=3;
    else
        b=4;

    printf("The number %d has %d digits\n",a,b);

    return 0;
}