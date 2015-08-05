#include <stdio.h>
#include <stdlib.h>

int* getRow(int rowIndex, int* returnSize) 
{
	int pre_returnSize;
	int i;
	int* tmp;
	if (rowIndex == 0)
	{
		*returnSize = 1;
		tmp = (int*)malloc(sizeof(int));
		tmp[0] = 1;
		return tmp;
	}
	if (rowIndex == 1)
	{
		*returnSize = 2;
		tmp = (int*)malloc(2 * sizeof(int));
		tmp[0] = 1;
		tmp[1] = 1;
		return tmp;
	}
	int* pre_nums = getRow(rowIndex - 1, &pre_returnSize);
	int* nums = (int*)malloc((pre_returnSize + 1) * sizeof(int));
	nums[0] = 1;
	for (i = 1; i < pre_returnSize; i++)
	{
		nums[i] = pre_nums[i - 1] + pre_nums[i];
	}
	nums[pre_returnSize] = 1;
	*returnSize = pre_returnSize + 1;
	return nums;
}

int main()
{
	int size;
	int* nums = getRow(10, &size);
	for (int i = 0; i < size; i++)
	{
		printf_s("%d  ", nums[i]);
	}
	printf_s("\n");
}