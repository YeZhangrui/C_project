#include<stdio.h>
int main(void)
{
    int y1,y2,m1,m2,d1,d2;

    printf("Enter first date:");
    scanf("%d/%d/%d",&m1,&d1,&y1);
    printf("Enter second date:");
    scanf("%d/%d/%d",&m2,&d2,&y2);

    // if (y1>y2||y1==y2,m1>m2||y1==y2,m1==m2,d1>d2)逻辑错误，","滥用
    if (y1>y2||(y1==y2&&m1>m2)||(y1==y2&&m1==m2&&d1>d2))
        printf("%.2d/%.2d/%.2d is later than %.2d/%.2d/%.2d\n",m1,d1,y1,m2,d2,y2);
    else 
        printf("%.2d/%.2d/%.2d is later than %.2d/%.2d/%.2d\n",m2,d2,y2,m1,d1,y1); 

    return 0;
}