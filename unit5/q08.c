#include<stdio.h>
int main(void)
{
    int hour,minute,time;
    printf("Enter a 24-hour time:");
    scanf("%d:%d",&hour,&minute);

    time=hour*60+minute;

    if (time<412)
    printf("Closest departure time is 8:00 a.m., arrive at 10:16 a.m.");
    else if(time<631)
    printf("Closest departure time is 9:43 a.m., arrive at 11:52 a.m.");
    else if(time<723)
    printf("Closest departure time is 11:19 a.m., arrive at 1:31 p.m.");
    else if(time<804)
    printf("Closest departure time is 12:47 p.m., arrive at 3:00 p.m.");
    else if(time<893)
    printf("Closest departure time is 2:00 p.m., arrive at 4:08p.m.");
    else if(time<1043)
    printf("Closest departure time is 3:45 p.m., arrive at 5:55p.m.");
    else if(time<1233)
    printf("Closest departure time is 7:00 p.m., arrive at 9:20p.m.");
    else 
    printf("Closest departure time is 9:45 p.m., arrive at 11:58p.m.");

    return 0;
}