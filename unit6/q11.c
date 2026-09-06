#include <stdio.h>
int main(void)
{
    int n;
    float a=1;
    float e=1;
    float i=1;

    printf("n=");
    scanf("%d",&n);

    for(;i<=n;i++)
    {
        a=a*1.00f/i;
        e=e+a;
    }

    printf("e=%f\n",e);

    return 0;
}