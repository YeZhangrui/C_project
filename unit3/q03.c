#include <stdio.h>
int main(void)
{
    int gis,group,publisher,item,check;
    printf("Enter ISBN:");
    scanf("%d-%d-%d-%d-%d",&gis,&group,&publisher,&item,&check);
    printf("GIS prefix:%d\n",gis);
    printf("Group identifier:%d\n",group);
    printf("Publisher code:%d\n",publisher);
    printf("Item number:%d\n",item);
    printf("Check digit:%d\n",check);
    return 0;
} 