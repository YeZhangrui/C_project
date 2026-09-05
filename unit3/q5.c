#include <stdio.h>
int main(void)
{
    int a1,a2,a3,a4,b1,b2,b3,b4,c1,c2,c3,c4,d1,d2,d3,d4;
    printf("Enter the number from 1 to 16 in nay order:");
    scanf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",&a1,&a2,&a3,&a4,&b1,&b2,&b3,&b4,&c1,&c2,&c3,&c4,&d1,&d2,&d3,&d4);
    printf("%d\t%d\t%d\t%d\t\n%d\t%d\t%d\t%d\t\n%d\t%d\t%d\t%d\t\n%d\t%d\t%d\t%d\t\n",a1,a2,a3,a4,b1,b2,b3,b4,c1,c2,c3,c4,d1,d2,d3,d4);
    int rowsumsa=a1+a2+a3+a4;
    int rowsumsb=b1+b2+b3+b4;
    int rowsumsc=c1+c2+c3+c4;
    int rowsumsd=d1+d2+d3+d4;
    printf("Row sums: %d %d %d %d\n",rowsumsa,rowsumsb,rowsumsc,rowsumsd);
    int columsums1=a1+b1+c1+d1;
    int columsums2=a2+b2+c2+d2;
    int columsums3=a3+b3+c3+d3;
    int columsums4=a4+b4+c4+d4;
    printf("Colum sums: %d %d %d %d\n",columsums1,columsums2,columsums3,columsums4);
    int digonalsums1=a1+b2+c3+d4;
    int digonalsums2=a4+b3+c2+d1;
    printf("Digonal sums:%d %d\n",digonalsums1,digonalsums2);
    return 0;
} 