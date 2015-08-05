#include <stdio.h>
#include <stdlib.h>

int countPrimes(int n) 
{
	if (n < 2)
	{
		return 0;
	}
	int i;
	int j = 2;
	int* pInt = (int*)malloc(n * sizeof(int));
	for (i = 0; i < n; i++)
	{
		pInt[i] = 1;
	}
	for (i = 2; i < n; i++)
	{
		if (pInt[i] == 0)
		{
			continue;
		}
		while (j * i < n)
		{
			pInt[j * i] = 0;
			j++;
		}
		j = 2;
	}
	int sum = 0;
	for (i = 0; i < n; i++)
	{
		sum += pInt[i];
	}
	return (sum - 2);
}

int main()
{
	printf_s("The result is: %d\n", countPrimes(121));
	return 0;
}