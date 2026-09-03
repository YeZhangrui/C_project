#include <stdio.h>
int main()
{
    int a,b,c;
    scanf("%d %d %d",&a,&b,&c);
    if(a+b<=c&&a+c<=b&&c+b<=a)
    {printf("erro");}
    else if((a==b)&&(a==c))
    {printf("dengbiansjx\n");}
    else if(a==b||a==c||c==b)
    {printf("dengyaosjx\n");}
    else{printf("putongsjx");}
    return 0;
}