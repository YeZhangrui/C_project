#include<stdio.h>
int main(void)
{
    float tax,income;

    printf("Enter income:");
    scanf("%f",&income);

    if (income<750)
    tax=income*0.01f;
    else if(income<2250)
    tax=7.5f+(income-750)*0.02f;
    else if(income<3750)
    tax=37.5f+(income-2250)*0.03f;
    else if(income<5250.0f)
    tax=82.5f+(income-3750)*0.04f;
    else if(income<7000.0f)
    tax=142.5f+(income-5250)*0.05f;
    else
    tax=230+(income-7000)*0.06f;

    printf("%.2f",tax);

    return 0;
}