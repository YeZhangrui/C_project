#include <stdio.h>
int main(void)
{   
    float loan0;
    printf("Enter amount of loan:");
    scanf("%f",&loan0);
    float rate;
    printf("Enter interest rate:");
    scanf("%f",&rate);
    float payment;
    printf("Enter monthly payment:");
    scanf("%f",&payment);
    // first month
    float loan1=loan0-payment+loan0*rate*0.01f/12;
    printf("Balance remaining after first payment:%0.2f\n",loan1);   
    // second month
    float loan2=loan1-payment+loan1*rate*0.01f/12;
    printf("Balance remaining after second payment:%0.2f\n",loan2);
    // thiird month
    float loan3=loan2-payment+loan2*rate*0.01f/12;
    printf("Balance remaining after third payment:%0.2f\n",loan3);
    return 0;
}