#include <stdio.h>
#include <stdlib.h>


//static int a[810] = { 0 };
//
//bool isHappy(int n) 
//{
//	int b[10] = { 0 };
//	int i = 0;
//	int j;
//	if (n == 1)
//	{
//		return true;
//	}
//	while (n > 0)
//	{
//		b[i] = n % 10;
//		n = n / 10;
//		i++;
//	}
//	int sum = 0;
//	for (j = 0; j < i; j++)
//	{
//		sum += b[j] * b[j];
//	}
//	if (a[sum])
//	{
//		return false;
//	}
//	a[sum] = 1;
//	return isHappy(sum);
//}



bool isHappy(int n)  //in [1, 11], only 1, n, 10 are happy numbers
{
	while (n > 12)
	{
		int next = 0;
		while (n)
		{
			next += (n % 10)*(n % 10);
			n /= 10;
		}
		n = next;
	}
	return ((n == 1) || (n == 7) || (n == 10));
}

int main()
{
	printf_s("The result is: %s\n", isHappy(10001) ? "TRUE" : "FLASE");

	return 0;
}