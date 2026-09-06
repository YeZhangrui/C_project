#include <stdio.h>

int main(void)
{
    float commission, value, shares, amount, other_commission;

    printf("Enter the number of stocks and the price per share: ");
    scanf("%f %f", &amount,&shares);
    value=shares*amount;

    if (value < 2500.00f)
    commission = 30.00f + .017f * value;
    else if (value < 6250.00f)
    commission = 56.00f + .0066f * value;
    else if (value < 20000.00f)
    commission = 76.00f + .0034f * value;
    else if (value < 50000.00f)
    commission = 100.00f + .0022f * value;
    else if (value < 500000.00f)
    commission = 155.00f + .0011f * value;
    else
    commission = 255.00f + .0009f * value;

    if (commission < 39.00f)
    commission = 39.00f;

    if(amount<2000.00f)
    other_commission=33.03f*amount;
    else
    other_commission=33.03f*amount;

    printf("Commission: $%.2f\nOther_commission: $%.2f\n", commission,other_commission);

    return 0;
}
