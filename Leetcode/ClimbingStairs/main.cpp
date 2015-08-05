#include <stdio.h>
#include <stdlib.h>

static int steps[100] = { 0 };

int climbStairs(int n) {
	if (n == 1)
	{
		steps[1] = 1;
		return 1;
	}
	if (n == 2)
	{
		steps[2] = 2;
		return 2;
	}
	if (steps[n] != 0)
	{
		return steps[n];
	}
	int step = climbStairs(n - 1) + climbStairs(n - 2);
	steps[n] = step;
	return step;
}

int main()
{
	printf_s("The result is: %d\n", climbStairs(41));
	return 0;
}