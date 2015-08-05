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
int minDepth(struct TreeNode* root) {
	if (root == NULL)
		return 0;
	if (root->left == NULL)
		return minDepth(root->right) + 1;
	if (root->right == NULL)
		return minDepth(root->left) + 1;
	int dep_l = minDepth(root->left);
	int dep_r = minDepth(root->right);
	return ((dep_l < dep_r) ? dep_l : dep_r) + 1;
}

int main()
{
	struct TreeNode* root = CreateTree(5);
	struct TreeNode* tmp1 = CreateTree(7);
	struct TreeNode* tmp2 = CreateTree(2);
	struct TreeNode* tmp3 = CreateTree(11);
	AddTreeLeaf(tmp3, tmp1, tmp2);
	tmp1 = CreateTree(4);
	AddTreeLeaf(tmp1, tmp3, NULL);
	tmp2 = CreateTree(1);
	tmp3 = CreateTree(4);
	AddTreeLeaf(tmp3, NULL, tmp2);
	tmp2 = CreateTree(13);
	struct TreeNode* tmp4 = CreateTree(8);
	AddTreeLeaf(tmp4, tmp2, tmp3);
	AddTreeLeaf(root, tmp1, tmp4);
	printf_s("The result is: %d\n", minDepth(root));
	return 0;
}