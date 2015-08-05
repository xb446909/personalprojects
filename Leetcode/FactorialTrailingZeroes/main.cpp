#include <stdio.h>
#include <stdlib.h>

int trailingZeroes(int n)
{
	int num = 0;
	n = n / 5;
	num += n;
	while (n > 1)
	{
		n = n / 5;
		num += n;
	}
	return num;
}

int main()
{
	printf_s("The result is: %d\n", trailingZeroes(15));
	return 0;
}