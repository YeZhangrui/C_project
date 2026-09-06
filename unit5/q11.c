#include<stdio.h>
int main(void)
{
    int number,i1,i2;

    printf("Enter a two-digit number:");
    scanf("%d",&number);

    i1=number/10;
    i2=number%10;

    if (i1!=1)
    {
        switch (i1)
        {
            case 2:
                printf("You enter the number twenty");
                break;
            case 3:
                printf("You enter the number thirty");
                break;
            case 4:
                printf("You enter the number forty");
                break;
            case 5:
                printf("You enter the number fifty");
                break;
            case 6:
                printf("You enter the number sixty");
                break;
            case 7:
                printf("You enter the number seventy");
                break;
            case 8:
                printf("You enter the number eighty");
                break;
            case 9:
                printf("You enter the number ninety");
                break;
        }
        switch (i2)
        {
            case 0:break;
            case 1:printf("-one");break;
            case 2:printf("-two");break;
            case 3:printf("-three");break;
            case 4:printf("-four");break;
            case 5:printf("-five");break;
            case 6:printf("-six");break;
            case 7:printf("-seven");break;
            case 8:printf("-eight");break;
            case 9:printf("-nine");break;
        }
    }
    else
        switch(i2) {
            case 0:printf("You enter the number ten");break;
            case 1:printf("You enter the number eleven");break;
            case 2:printf("You enter the number twelve");break;
            case 3:printf("You enter the number thirteen");break;
            case 4:printf("You enter the number fourteen");break;
            case 5:printf("You enter the number fifteen");break;
            case 6:printf("You enter the number sixteen");break;
            case 7:printf("You enter the number seventeen");break;
            case 8:printf("You enter the number eighteen");break;
            case 9:printf("You enter the number nineteen");break;
        }
    return 0;
}