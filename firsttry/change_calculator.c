#include<stdio.h>
int main()
{//初始化
	int price = 0;
    int amount =0;
//读入金额和票面
	printf("请输入金额: ");
	scanf("%d", &price);

    printf("请输入面额: ");
	scanf("%d", &amount);
//计算找零
	int change = amount - price;
//判断金额是否足够
    if(change<0)
    {
        printf("金额不足\n");
        return 0;
    }

	printf("找您%d元\n", change);

	return 0;
}