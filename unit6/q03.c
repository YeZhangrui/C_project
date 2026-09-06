#include <stdio.h>
int main(void)
{
    int m,n,r,a,b;
    printf("Enter a fraction:");
    scanf("%d/%d",&m,&n);

    a=m;
    b=n;
    
    while(n!=0){
        r=m%n;
        m=n;
        n=r;
    }

    a=a/m;
    b=b/m;

    printf("In lowest terms:%d/%d",a,b);
   
    return 0;
}