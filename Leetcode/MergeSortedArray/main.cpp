#include <stdio.h>
#include <stdlib.h>

#define NUMS1_LEN	10
#define NUMS2_LEN	5

void merge(int* nums1, int m, int* nums2, int n) 
{
	int i;
	if (n == 0)
	{
		return;
	}
	if (m == 0)
	{
		for (i = 0; i < n; i++)
		{
			nums1[i] = nums2[i];
		}
		return;
	}
	i = m + n - 1;
	while ((m > 0) && (n > 0))
	{
		if (nums1[m - 1] > nums2[n - 1])
		{
			nums1[i] = nums1[m - 1];
			m--;
		}
		else
		{
			nums1[i] = nums2[n - 1];
			n--;
		}
		i--;
	}
	if (n != 0)
	{
		while (n > 0)
		{
			nums1[i] = nums2[n - 1];
			n--;
			i--;
		}
	}
	if (m != 0)
	{
		while (m > 0)
		{
			nums1[i] = nums1[m - 1];
			m--;
			i--;
		}
	}
}

int main()
{
	int i;
	int nums1[NUMS1_LEN] = { 0, 2, 4, 6, 8 };
	int nums2[NUMS2_LEN] = { 1, 3, 5, 7, 9 };
	printf_s("The nums1: ");
	for (i = 0; i < (NUMS1_LEN - NUMS2_LEN); i++)
	{
		printf_s("%d  ", nums1[i]);
	}
	printf_s("\nThe nums2: ");
	for (i = 0; i < NUMS2_LEN; i++)
	{
		printf_s("%d  ", nums2[i]);
	}
	merge(nums1, NUMS1_LEN - NUMS2_LEN, nums2, NUMS2_LEN);
	printf_s("\nThe result is: ");
	for (i = 0; i < NUMS1_LEN; i++)
	{
		printf_s("%d  ", nums1[i]);
	}
	printf_s("\n");
	return 0;
}