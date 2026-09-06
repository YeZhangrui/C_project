#include <stdio.h>
int main(void)
{
    int item,m,d,y;
    float unit;
    printf("Enter item number:");
    scanf("%d",&item);
    printf("Enter unit price:");
    scanf("%f",&unit);
    printf("Enter purchase date (mm/dd/yyyy):");
    scanf("%d/%d/%d",&m,&d,&y);
    printf("Item\tUnit\tPurchase\n\tPrice\tDate\n%d\t$%7.2f\t%.2d/%.2d/%d\n",item,unit,m,d,y);
    return 0;
} 