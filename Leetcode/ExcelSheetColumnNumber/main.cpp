#include <stdio.h>
#include <stdlib.h>

int titleToNumber(char* s)
{
	int num = 0;
	int len = 0;
	while (s[len] != '\0')
	{
		num *= 26;
		num += s[len] - 'A' + 1;
		len++;
	}
	return num;
}

int main()
{
	printf_s("The result is: %d\n", titleToNumber("AZZA"));
	return 0;
}