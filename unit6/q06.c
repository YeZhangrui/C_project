#include <stdio.h>
int main(void)
{
    int n,i;
    scanf("%d",&n);

    for(i=1;;i++){
        if(n<4*i*i)
        break;
        printf("%d\n",4*i*i);
    }

    return 0;
}