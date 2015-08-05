#include <stdio.h>
#include <stdlib.h>

char* addBinary(char* a, char* b) 
{
	int num_a = 0;
	int num_b = 0;
	int num_c = 0;
	int i = 0;
	char* c = (char*)malloc(33);
	while ((a[i] == '0') || (a[i] == '1'))
	{
		num_a = num_a << 1;
		if (a[i] == '1')
		{
			num_a += 1;
		}
		i++;
	}
	i = 0;
	while ((b[i] == '0') || (b[i] == '1'))
	{
		num_b = num_b << 1;
		if (b[i] == '1')
		{
			num_b += 1;
		}
		i++;
	}
	num_c = num_a + num_b;
	for (i = 0; i < 33; i++)
	{
		c[i] = '\0';
	}
	i = 31;
	if (num_c == 0)
	{
		c[i] = '0';
		return &c[i];
	}
	while (num_c > 0)
	{
		if (num_c & 0x1)
		{
			c[i] = '1';
		}
		else
		{
			c[i] = '0';
		}
		i--;
		num_c >>= 1;
	}
	return &c[i + 1];
}

int main()
{
	char* a = "011";
	char* b = "1";
	char* c = addBinary(a, b);
	printf_s("The result is: %s\n", c);
	return 0;
}