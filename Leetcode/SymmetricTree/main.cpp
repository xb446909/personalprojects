#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
	int val;
	struct TreeNode* left;
	struct TreeNode* right;
};

struct TreeNode* CreateTree(int val)
{
	struct TreeNode* root = (struct TreeNode*)malloc(sizeof(struct TreeNode));
	root->val = val;
	root->left = NULL;
	root->right = NULL;
	return root;
}

void  AddTreeLeaf(struct TreeNode* root, struct TreeNode* leftleaf, struct TreeNode* rightleft)
{
	root->left = leftleaf;
	root->right = rightleft;
}

int* middle_order(struct TreeNode* root, int* array_num, int null_num)
{
	int array_num_l, array_num_r;
	int i;	
	if (root == NULL)
	{
		*array_num = 1;
		int* num = (int*)malloc(sizeof(int));
		num[0] = null_num;
		return num;
	}
	if ((root->left == NULL) && (root->right == NULL))
	{
		*array_num = 1;
		int* num = (int*)malloc(sizeof(int));
		num[0] = root->val;
		return num;
	}
	int* nums_l = middle_order(root->left, &array_num_l, null_num);
	int* nums_r = middle_order(root->right, &array_num_r, null_num);
	*array_num = array_num_l + array_num_r + 1;
	int* nums = (int*)malloc(*array_num * sizeof(int));
	for (i = 0; i < array_num_l; i++)
	{
		nums[i] = nums_l[i];
	}
	nums[array_num_l] = root->val;
	for (i = 0; i < array_num_r; i++)
	{
		nums[i + array_num_l + 1] = nums_r[i];
	}
	return nums;
}

bool isChildSymmetric(struct TreeNode* left_child, struct TreeNode* right_child)
{
	if ((left_child == NULL) && (right_child == NULL))
	{
		return true;
	}
	if ((left_child != NULL) && (right_child != NULL))
	{
		if (left_child->val != right_child->val)
		{
			return false;
		}
		else
		{
			return (isChildSymmetric(left_child->left, right_child->right) &&
				isChildSymmetric(left_child->right, right_child->left));
		}
	}
	return false;
}

/**
* Definition for a binary tree node.
* struct TreeNode {
*     int val;
*     struct TreeNode *left;
*     struct TreeNode *right;
* };
*/
bool isSymmetric(struct TreeNode* root) {
	/*int size;
	int null_num = 0x7fffffff;
	if (root == NULL)
	{
		return true;
	}
	if ((root->left != NULL) && (root->right != NULL))
	{
		if (root->left->val != root->right->val)
		{
			return false;
		}
	}
	int* nums = middle_order(root, &size, null_num);
	if (size % 2 == 0)
	{
		return false;
	}
	if (root)
		for (int i = 0; i < (size >> 1); i++)
		{
			if (nums[i] != nums[size - 1 - i])
			{
				return false;
			}
		}
	return true;*/
	if (root == NULL)
	{
		return true;
	}
	return isChildSymmetric(root->left, root->right);
}

/*
            1                              1
		  /   \                           / \
		 2     2                         2   3
        / \   / \                       /   / 
	   3   4 4   3                     3   2
*/

int main()
{
	struct TreeNode* root = CreateTree(1);
	struct TreeNode* node1 = CreateTree(2);
	struct TreeNode* node2 = CreateTree(2);
	struct TreeNode* node3 = CreateTree(3);
	struct TreeNode* node4 = CreateTree(3);
	struct TreeNode* node5 = CreateTree(4);
	struct TreeNode* node6 = CreateTree(4);
	AddTreeLeaf(root, node1, node2);
	AddTreeLeaf(node1, node3, node5);
	AddTreeLeaf(node2, node6, node4);
	printf_s("The result is: %s\n", isSymmetric(root) ? "TRUE" : "FLASE");

	root = CreateTree(1);
	node1 = CreateTree(2);
	node2 = CreateTree(2);
	node3 = CreateTree(3);
	node4 = CreateTree(3);

	AddTreeLeaf(root, node1, node3);
	AddTreeLeaf(node1, node4, NULL);
	AddTreeLeaf(node3, node2, NULL);
	printf_s("The result is: %s\n", isSymmetric(root) ? "TRUE" : "FLASE");

	return 0;
}