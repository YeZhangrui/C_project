#include <stdio.h>

int main(void)
{
  int i, n, odd, square;

  printf("This program prints a table of squares.\n");
  printf("Enter number of entries in table: ");
  scanf("%d", &n);

  i = 1;
  square=1;
  for (i = 1; i <= n; i++) {
    printf("%10d%10d\n", i, square);
    square =square+i+i+1;
  }

  return 0;
}