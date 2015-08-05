#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode
{
	int val;
	struct ListNode* next;
}*pListNode, ListNode;

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

/**
* Definition for singly-linked list.
* struct ListNode {
*     int val;
*     struct ListNode *next;
* };
*/
struct ListNode* removeElements(struct ListNode* head, int val)
{
	while (head && (head->val == val))
	{
		head = head->next;
	}
	if (!head)
	{
		return NULL;
	}
	struct ListNode* pNode = head;
	while (pNode->next)
	{
		if (pNode->next->val == val)
		{
			pNode->next = pNode->next->next;
			continue;
		}
		pNode = pNode->next;
	}
	return head;
}

int main()
{
	ListNode a1, a2, a3, a4, a5;
	a1.next = &a2;
	a1.val = 2;
	a2.next = &a3;
	a2.val = 2;
	a3.next = &a4;
	a3.val = 2;
	a4.next = &a5;
	a4.val = 2;
	a5.next = NULL;
	a5.val = 2;
	PrintList(&a1);

	PrintList(removeElements(&a1, 2));

	return 0;
}