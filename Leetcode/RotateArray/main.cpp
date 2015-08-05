#include <stdio.h>
#include <stdlib.h>

void rotate(int* nums, int numsSize, int k) 
{
	int i;
	int newk = numsSize - (k % numsSize);
	int* pNums = (int*)malloc(2 * numsSize * sizeof(int));
 	for (i = 0; i < numsSize; i++)
	{
		pNums[i] = nums[i];
		pNums[i + numsSize] = nums[i];
	}
	for (i = 0; i < numsSize; i++)
	{
		nums[i] = pNums[i + newk];
	}
}

int main()
{
	int nums[7] = { 1, 2, 3, 4, 5, 6, 7 };
	int length = 7;
	rotate(nums, 7, 3);
	for (size_t i = 0; i < length; i++)
	{
		printf_s("%d  ", nums[i]);
	}
	printf_s("\n");
	return 0;
}