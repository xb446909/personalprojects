#include <stdio.h>
#include <stdlib.h>

int hammingWeight(unsigned int n)
{
	int num = 0;
	while (n > 0)
	{
		if (n & 0x00000001)
		{
			num++;
		}
		n = n >> 1;
	}
	return num;
}

int main()
{
	printf_s("The result is: %d\n", hammingWeight(11));
	return 0;
}