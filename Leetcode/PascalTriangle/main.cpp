#include <stdio.h>
#include <stdlib.h>

/**
* Return an array of arrays of size *returnSize.
* The sizes of the arrays are returned as *columnSizes array.
* Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
*/
int** generate(int numRows, int** columnSizes, int* returnSize) 
{
	int i, j;
	int** nums = (int**)malloc(numRows * sizeof(int*));
	*returnSize = 0;
	for (i = 0; i < numRows; i++)
	{
		columnSizes[i] = (int*)malloc(sizeof(int));
		*columnSizes[i] = (i + 1) * sizeof(int);
		nums[i] = (int*)malloc((i + 1) * sizeof(int));
		*returnSize += (i + 1)  * sizeof(int);
	}
	if (numRows >= 1)
	{
		nums[0][0] = 1;
	}
	if (numRows >= 2)
	{
		nums[1][0] = 1;
		nums[1][1] = 1;
	}
	if (numRows > 2)
	{
		for (i = 2; i < numRows; i++)
		{
			nums[i][0] = 1;
			nums[i][i] = 1;
			for (j = 1; j < i; j++)
			{
				nums[i][j] = nums[i - 1][j - 1] + nums[i - 1][j];
			}
		}
	}

	return nums;
}

int main()
{
	int i, j;
	int** nums;
	int** columnSizes = NULL;
	int numRows = 1;
	int returnSize = 0;
	columnSizes = (int**)malloc(numRows * sizeof(int*));
	nums = generate(numRows, columnSizes, &returnSize);
	printf_s("columnSize: \n");
	for (i = 0; i < numRows; i++)
	{
		printf_s("%d  ", *columnSizes[i]);
	}
	printf_s("\n");
	printf_s("returnSize = %d\n", returnSize);
	printf_s("Numbers: \n");
	for (i = 0; i < numRows; i++)
	{
		for (j = 0; j  < i + 1; j ++)
		{
			printf_s("%d  ", nums[i][j]);
		}
		printf_s("\n");
	}
}