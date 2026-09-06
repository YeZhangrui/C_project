#include <stdio.h>
int main(void)
{
    float a=1;
    float e=1;
    float i=1;
    float E=0;

    printf("E=");
    scanf("%f",&E);

    for(;a>E;i++)
    {
        a=a*1.00f/i;
        e=e+a;
    }
       
    printf("e=%f\n",e);

    return 0;
}