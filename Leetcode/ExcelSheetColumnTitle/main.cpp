#include <stdio.h>
#include <stdlib.h>

char* convertToTitle(int n) 
{
	char* title = (char*)malloc(9);
	title[8] = '\0';
	int i = 7;
	while (n > 0)
	{
		title[i--] = (n % 26) + 'A' - 1;
		if (title[i + 1] == 'A' - 1)
		{
			title[i + 1] = 'Z';
			n -= 26;
		}
		n /= 26;
	}
	return &title[i + 1];
}

int main()
{
	printf_s("The result is: %s\n", convertToTitle(35829));
	return 0;
}