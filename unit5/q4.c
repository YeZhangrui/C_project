#include<stdio.h>
int main(void)
{
    int speed;
    printf("Enter the wind speed:");
    scanf("%d",&speed);

    if(speed<1)
    printf("Clame");
    else if(speed<=3)
    printf("Light air");
    else if(speed<=27)
    printf("Breeze");
    else if(speed<=47)
    printf("Gale");
    else if(speed<=63)
    printf("Storm");
    else
    printf("Hurricane");

    return 0;
}