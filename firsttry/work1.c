// 题目描述
// 输入三个不同的整数（保证不相等），输出其中的最大值。
// 要求：只能使用 if 或 else if，不能嵌套。

// 示例
// 输入：5 12 8
// 输出：最大值为 12
#include <stdio.h>
int main()
{
    int a=0;
    int b=0;
    int c=0;
    scanf("%d %d %d",&a,&b,&c);
   
    if (a>b&&a>c)
    {
        printf("MAX:%d\n",a);/* code */
    }
    if (b>a&&b>c)
    {
        printf("MAX:%d\n",b);/* code */
    }
    if (c>b&&c>a)
    {
        printf("MAX:%d\n",c);/* code */
    }
}