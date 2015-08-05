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
struct ListNode *getIntersectionNode(struct ListNode *headA, struct ListNode *headB) 
{
	struct ListNode *currA = headA;
	struct ListNode *currB = headB;
	struct ListNode *tmp;

	while (currA && currB)
	{
		currA = currA->next;
		currB = currB->next;
	}
	if (currA)
	{
		tmp = headA;
		while (currA)
		{
			currA = currA->next;
			tmp = tmp->next;
		}
		currB = headB;
		while (currB != tmp)
		{
			currB = currB->next;
			tmp = tmp->next;
		}
		return tmp;
	}
	else
	{
		tmp = headB;
		while (currB)
		{
			currB = currB->next;
			tmp = tmp->next;
		}
		currA = headA;
		while (currA != tmp)
		{
			currA = currA->next;
			tmp = tmp->next;
		}
		return tmp;
	}
}

int main()
{
	ListNode a1, a2, a3, a4, a5;
	a1.next = &a2;
	a1.val = 1;
	a2.next = &a3;
	a2.val = 2;
	a3.next = &a4;
	a3.val = 3;
	a4.next = &a5;
	a4.val = 4;
	a5.next = NULL;
	a5.val = 5;
	PrintList(&a1);

	PrintList(getIntersectionNode(&a1, &a2));

	return 0;
}