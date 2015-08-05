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

int depth(struct TreeNode* root)
{
	if (root == NULL)
	{
		return 0;
	}
	int dep_l = depth(root->left);
	int dep_r = depth(root->right);
	return ((dep_l > dep_r) ? dep_l : dep_r) + 1;
}

/**
* Definition for a binary tree node.
* struct TreeNode {
*     int val;
*     struct TreeNode *left;
*     struct TreeNode *right;
* };
*/
bool isBalanced(struct TreeNode* root) {
	if (root == NULL)
	{
		return true;
	}
	int dep_l = depth(root->left);
	int dep_r = depth(root->right);
	if (((dep_l - dep_r) >= -1) && ((dep_l - dep_r) <= 1))
	{
		return (isBalanced(root->left) && isBalanced(root->right));
	}
	return false;
}

/*
         5
	   /   \
	  4      8
	 / \    / \
	11 11  13  4
   /  \         \
  7    2         1
*/

int main()
{
	struct TreeNode* root = CreateTree(5);
	struct TreeNode* tmp1 = CreateTree(7);
	struct TreeNode* tmp2 = CreateTree(2);
	struct TreeNode* tmp3 = CreateTree(11);
	struct TreeNode* tmp4 = CreateTree(11);
	AddTreeLeaf(tmp3, tmp1, tmp2);
	tmp1 = CreateTree(4);
	AddTreeLeaf(tmp1, tmp3, tmp4);
	tmp2 = CreateTree(1);
	tmp3 = CreateTree(4);
	AddTreeLeaf(tmp3, NULL, tmp2);
	tmp2 = CreateTree(13);
	tmp4 = CreateTree(8);
	AddTreeLeaf(tmp4, tmp2, tmp3);
	AddTreeLeaf(root, tmp1, tmp4);
	printf_s("The result is: %s\n", isBalanced(root) ? "TRUE" : "FLASE");
	return 0;
}