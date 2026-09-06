#include <stdio.h>
int main(void)
{   
    float loan;
    printf("Enter amount of loan:");
    scanf("%f",&loan);

    float rate;
    printf("Enter interest rate:");
    scanf("%f",&rate);

    float payment;
    printf("Enter monthly payment:");
    scanf("%f",&payment);

    int num_of_payments;
    printf("Enter the num of payments:");
    scanf("%d",&num_of_payments);

    int i=1;

    for(;i<=num_of_payments;i++)
    {
        loan=loan-payment+loan*rate*0.01f/12;
        if(loan>0)
        printf("Balance remaining after payment %d: $%.2f\n",i, loan);
        else
        printf("Balance remaining after payment %d: $00.00\n",i);
    }
    return 0;
}