#include <stdio.h>
int main(void)
{
    int a;
    printf("Enter a number between 0 and 32767:");
    scanf("%d",&a);
    int i5=a%8;
    int i4=a/8%8;
    int i3=a/64%8;
    int i2=a/512%8;
    int i1=a/4096%8;
    printf("In octal, your number is:%d%d%d%d%d\n",i1,i2,i3,i4,i5);

    return 0;
}
