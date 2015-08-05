#include <stdio.h>
#include <stdlib.h>

unsigned int reverseBits(unsigned int n)
{
	int bits[32] = { 0 };
	unsigned int m = 0;
	int i = 31;
	int j;
	while (n > 0)
	{
		bits[i] = n & 0x00000001;
		i--;
		n = n >> 1;
	}
	for (j = i + 1; j < 32; j++)
	{
		if (bits[j])
		{
			m |= (0x000000001 << j);
		}
	}
	return m;
}

int main()
{
	printf_s("The result is: 0x%x\n", reverseBits(0x80000000));
	return 0;
}