#include <stdio.h>
#include <stdlib.h>

int majorityElement(int* nums, int numsSize)
{
	int major;
	int count = 0;
	int i;
	for (i = 0; i < numsSize; i++)
	{
		if (count == 0)
		{
			major = nums[i];
			count++;
		}
		else if (major == nums[i])
		{
			count++;
		}
		else
		{
			count--;
		}
	}
	return major;
}

int main()
{
	int nums[5] = { 1, 1, 1, 2, 2 };
	printf_s("The result is: %d\n", majorityElement(nums, 5));
	return 0;
}