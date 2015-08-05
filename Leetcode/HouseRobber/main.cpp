#include <stdio.h>
#include <stdlib.h>

int rob(int* nums, int numsSize) 
{
	int rob = 0;	//�������ǰ���ӣ��õ�����Ǯ��
	int notrob = 0;	//���������ǰ���ӣ��õ�����Ǯ��
	int currob = 0;
	int i;
	for (i = 0; i < numsSize; i++)
	{
		currob = notrob + nums[i];	//�����������ӣ���ôǰһ�����ӾͲ�������
									//���Եõ���Ǯ��notrob(ǰһ������û����ʱ��Ǯ)�͵�ǰ���ӵ�Ǯnums[i]
		notrob = (rob > notrob) ? rob : notrob;				//�������������ӣ���ô��Ϳ���ѡ��ǰһ�������������ǲ�����ѡ��Ǯ���
		rob = currob;				//rob�������˵�ǰ���ӵ�Ǯ��
	}
	return (rob > notrob) ? rob : notrob;	//�������Ͳ�����Ǯ������
}

int main()
{
	int nums[5] = { 1, 2, 3, 4, 5 };
	printf_s("The result is: %d\n", rob(nums, 5));

	return 0;
}