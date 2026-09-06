#include <stdio.h>
int main(void)
{
    int n,m;

    printf("Enter number of days in month:");
    scanf("%d",&n);
    printf("Enter starting day of the week (1=Sun,7=Sat):");
    scanf("%d",&m);

    for(int i=m-1;i!=0;i--)
    {
        printf("\t");
    }
    
    for(int a=1;n>0;n--)
    {
        if ((a+m-1)%7==0)
        {
            printf("%d\t\n",a);
        }
        else
        {
            printf("%d\t",a);
        }

        a++;
    }

    return 0;
}