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

/**
* Definition for a binary tree node.
* struct TreeNode {
*     int val;
*     struct TreeNode *left;
*     struct TreeNode *right;
* };
*/
bool isSameTree(struct TreeNode* p, struct TreeNode* q) {
	bool isSame = false;
	if ((p == NULL) && (q == NULL))
	{
		isSame = true;
	}
	if ((p != NULL) && (q != NULL))
	{
		if (p->val != q->val)
		{
			isSame = false;
		}
		else
		{
			isSame = (isSameTree(p->left, q->left) && isSameTree(p->right, q->right));
		}	
	}
	return isSame;
}

/*
//       1                              1
//     /   \                           / \
//    2     2                         2   3
//   / \   / \                       /   /
//  3   4 4   3                     3   2
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
	printf_s("The result is: %s\n", isSameTree(root, root) ? "TRUE" : "FLASE");

	struct TreeNode* root1 = CreateTree(1);
	node1 = CreateTree(2);
	node2 = CreateTree(2);
	node3 = CreateTree(3);
	node4 = CreateTree(3);

	AddTreeLeaf(root1, node1, node3);
	AddTreeLeaf(node1, node4, NULL);
	AddTreeLeaf(node3, node2, NULL);
	printf_s("The result is: %s\n", isSameTree(root, root1) ? "TRUE" : "FLASE");

	return 0;
}