#include<stdio.h>
int main(void)
{
    int y,m,d,y1,m1,d1;

    
    printf("Enter a date:");
    scanf("%d/%d/%d",&m,&d,&y);

    if (y==0||d==0||m==0)
        return 0;

    for (;;)
    {
        printf("Enter a date:");
        scanf("%d/%d/%d",&m1,&d1,&y1);
        if(y1==0||m1==0||d1==0)
        {
            break;
        }
        else if (y>y1||(y==y1&&m>m1)||(y==y1&&m==m1&&d>d1))
        {
            y=y1;
            m=m1;
            d=d1;
        }
        
    }
      
    printf("%.2d/%.2d/%.2d is earliest date\n",m,d,y); 

    return 0;
}