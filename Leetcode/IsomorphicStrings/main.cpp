#include <stdio.h>
#include <stdlib.h>

bool isIsomorphic(char* s, char* t)
{
	int f[255] = { 0 };
	int j[255] = { 0 };
	int len = 0;
	int i;
	while (s[len] != '\0')
	{
		len++;
	}
	for (i = 0; i < len; i++)
	{
		if (((f[s[i]] != 0) && (f[s[i]] != t[i])) ||
			((j[t[i]] != 0) && (j[t[i]] != s[i])))
		{
			return false;
		}
		if (f[s[i]] == 0)
		{
			f[s[i]] = t[i];
		}
		if (j[t[i]] == 0)
		{
			j[t[i]] = s[i];
		}
	}
	return true;
}

int main()
{
	char *s = "a";
	char *t = "b";
	printf_s("The result is: %s\n", isIsomorphic(s, t) ? "TRUE" : "FLASE");
	return 0;
}