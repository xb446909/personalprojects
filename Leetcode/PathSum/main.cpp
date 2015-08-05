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
bool hasPathSum(struct TreeNode* root, int sum) 
{
	if (root == NULL)
		return false;
	if ((root->left == NULL) && (root->right == NULL) && (root->val == sum))
		return true;
	return (hasPathSum(root->left, sum - root->val) || hasPathSum(root->right, sum - root->val));
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
	printf_s("The result is: %s\n", hasPathSum(root, 23) ? "TRUE" : "FLASE");
	return 0;
}