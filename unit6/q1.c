 #include <stdio.h>
int main(void)
{
    float n,max;
    printf("Enter a number:");
    scanf("%f",&n);
    max=n;
    
    while(n>0.0f)
    {
        printf("Enter a number:");
        scanf("%f",&n);
        if (n>max)
        {
        max=n;
        }
    }
    printf("MAX=%f",max);
    
    
    return 0;
}