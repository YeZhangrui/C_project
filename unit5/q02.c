#include<stdio.h>
int main(void)
{
    int a,b;

    printf("Enter a 24-hour time:");
    scanf("%d:%d",&a,&b);

    if(a<12)
    {
        printf("Equivalent 12-hour time: %d:%.2d AM",a,b);
    }
    else if(a=12)
    {
        printf("Equivalent 12-hour time: %d:%.2d PM",a,b);
    }
    else
    {
        a=a-12;
        printf("Equivalent 12-hour time: %d:%.2d PM",a,b);
    }

    return 0;
}