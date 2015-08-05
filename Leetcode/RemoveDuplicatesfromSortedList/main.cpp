#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode
{
	int val;
	struct ListNode* next;
}*pListNode, mListNode;

void PrintList(pListNode head)
{
	pListNode a = head;
	printf_s("Print List: ");
	while (a != NULL)
	{
		printf_s("->%d", a->val);
		a = a->next;
	}
	printf_s("\n");

}

pListNode CreateList(int* nums, int size)
{
	if (size <= 0)
	{
		return NULL;
	}
	pListNode head = (pListNode)malloc(sizeof(mListNode));
	head->val = nums[0];
	head->next = NULL;
	size--;
	pListNode tail = head;
	int i = 1;
	while (size-- > 0)
	{
		tail->next = (pListNode)malloc(sizeof(mListNode));
		tail->next->val = nums[i++];
		tail->next->next = NULL;
		tail = tail->next;	
	}
	return head;
}

struct Node
{
	unsigned int val;
	struct Node* next;
};

#define HASH_TABLE_SIZE		20

typedef struct _HashTable
{
	struct Node* value[HASH_TABLE_SIZE];
}HashTable, *pHashTable;

bool insertVal(pHashTable pTb, unsigned int val)
{
	int num = val % HASH_TABLE_SIZE;
	if (pTb->value[num] == NULL)
	{
		pTb->value[num] = (struct Node*)malloc(sizeof(struct Node));
		pTb->value[num]->next = NULL;
		pTb->value[num]->val = val;
	}
	else if (val == pTb->value[num]->val)
	{
		return false;
	}
	struct Node* pointer = pTb->value[num];
	while (pointer->next != NULL)
	{
		if (pointer->next->val == val)
		{
			return false;
		}
		pointer = pointer->next;
	}
	struct Node* tmp = (struct Node*)malloc(sizeof(struct Node));
	tmp->next = NULL;
	tmp->val = val;
	pointer->next = tmp;
	return true;
}

/**
* Definition for singly-linked list.
* struct ListNode {
*     int val;
*     struct ListNode *next;
* };
*/
struct ListNode* deleteDuplicates(struct ListNode* head) {
	int i;
	HashTable HashTb;
	for (i = 0; i < HASH_TABLE_SIZE; i++)
	{
		HashTb.value[i] = NULL;
	}
	if ((head == NULL) || (head->next == NULL))
	{
		return head;
	}
	struct ListNode* pointer = head;
	insertVal(&HashTb, (unsigned int)head->val);
	while (pointer->next != NULL)
	{
		if (insertVal(&HashTb, (unsigned int)pointer->next->val) == false)
		{
			pointer->next = pointer->next->next;
			continue;
		}
		pointer = pointer->next;
	}
	return head;
}

int main()
{
	int nums[] = { -3, -3, -3, -2, -1, -1, 0, 0, 0, 0, 0 };
	pListNode list = CreateList(nums, sizeof(nums) / sizeof(int));
	PrintList(list);

	PrintList(deleteDuplicates(list));

	return 0;
}