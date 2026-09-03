#include <stdio.h>
int main(void)
{
    int m,d,y;
    printf("Enter a date (mm/dd/yyyy):");
    scanf("%d/%d/%d",&m,&d,&y);
    if (m<10)
    printf("You entered the date %d0%d%d",y,m,d);
    else
    printf("You entered the date %d%d%d",y,m,d);
}