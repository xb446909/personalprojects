#include <stdio.h>
#include <stdlib.h>

int rob(int* nums, int numsSize) 
{
	int rob = 0;	//如果抢当前房子，得到最多的钱数
	int notrob = 0;	//如果不抢当前房子，得到最多的钱数
	int currob = 0;
	int i;
	for (i = 0; i < numsSize; i++)
	{
		currob = notrob + nums[i];	//如果抢这个房子，那么前一个房子就不能抢，
									//所以得到的钱是notrob(前一个房子没有抢时的钱)和当前房子的钱nums[i]
		notrob = (rob > notrob) ? rob : notrob;				//如果不抢这个房子，那么你就可以选择前一个房子是抢还是不抢，选择钱多的
		rob = currob;				//rob就是抢了当前房子的钱数
	}
	return (rob > notrob) ? rob : notrob;	//返回抢和不抢里钱数最多的
}

int main()
{
	int nums[5] = { 1, 2, 3, 4, 5 };
	printf_s("The result is: %d\n", rob(nums, 5));

	return 0;
}