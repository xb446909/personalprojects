#include <stdio.h>
#include <stdlib.h>

typedef struct _MinStack {
	int val;
	int num;
	struct _MinStack* next;
} MinStack;

void PrintList(MinStack* head)
{
	MinStack* a = head;
	printf_s("Print List: ");
	while (a != NULL)
	{
		printf_s("->%d", a->val);
		a = a->next;
	}
	printf_s("\n");

}

void minStackCreate(MinStack *stack, int maxSize) {
	stack->val = 0x7fffffff;
	stack->num = 0;
	stack->next = NULL;
}

void minStackPush(MinStack *stack, int element) {
	MinStack* tmp = (MinStack*)malloc(sizeof(MinStack));
	tmp->val = element;
	tmp->next = stack->next;
	stack->next = tmp;
	if (element < stack->val)
	{
		stack->val = element;
		stack->num = 1;
	}
	else if (element == stack->val)
	{
		stack->num++;
	}
}

void minStackPop(MinStack *stack) {
	MinStack* tmp = stack->next;
	stack->next = tmp->next;
	if (tmp->val == stack->val)
	{
		stack->num--;
		if (stack->num == 0)
		{
			stack->val = 0x7fffffff;
			tmp = stack->next;
			while (tmp != NULL)
			{
				if (tmp->val < stack->val)
				{
					stack->val = tmp->val;
					stack->num = 1;
				}
				else if (tmp->val == stack->val)
				{
					stack->num++;
				}
				tmp = tmp->next;
			}
		}
	}
}

int minStackTop(MinStack *stack) {
	return stack->next->val;
}

int minStackGetMin(MinStack *stack) {
	return stack->val;
}

void minStackDestroy(MinStack *stack) {
	stack->next = NULL;
}

int main()
{
	MinStack* stack = (MinStack*)malloc(sizeof(MinStack));
	minStackCreate(stack, 5);
	minStackPush(stack, 2);
	minStackPush(stack, 0);
	minStackPush(stack, 3);
	minStackPush(stack, 0);
	printf_s("%d ", minStackGetMin(stack));
	minStackPop(stack);
	printf_s("%d ", minStackGetMin(stack));
	minStackPop(stack);
	printf_s("%d ", minStackGetMin(stack));
	minStackPop(stack);
	printf_s("%d ", minStackGetMin(stack));
	printf_s("\n");
	return 0;
}