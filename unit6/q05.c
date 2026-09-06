#include <stdio.h>
int main(void)
{
    int a,i;
    printf("Enter a number:");
    scanf("%d",&a);

    printf("The reversal is:");
    
    do
    {
        printf("%d",a%10);
        a=a/10;
        i=a;
    }while(i!=0);

    return 0;
}