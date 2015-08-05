#include <stdio.h>
#include <stdlib.h>

int compareVersion(char* version1, char* version2) 
{
	int i = 0;
	int j = 0;
	int ver1 = 0;
	int ver2 = 0;
	if (version1[0] == '.')
	{
		i++;
	}
	if (version2[0] == '.')
	{
		j++;
	}
	while ((version1[i] != '.') && (version1[i] != '\0'))
	{
		ver1 *= 10;
		ver1 += version1[i] - '0';
		i++;
	}
	while ((version2[j] != '.') && (version2[j] != '\0'))
	{
		ver2 *= 10;
		ver2 += version2[j] - '0';
		j++;
	}
	if (ver1 != ver2)
	{
		return (ver1 > ver2) ? 1 : -1;
	}
	if ((version1[i] == '\0') && (version2[j] == '\0'))
	{
		return 0;
	}
	return compareVersion(&version1[i], &version2[j]);

}

int main()
{
	char* ver1 = "3.01";
	char* ver2 = "3.00";
	printf_s("The result is: %d\n", compareVersion(ver1, ver2));
}