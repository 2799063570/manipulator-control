#include <iostream>

using namespace std;

class TreeNode
{
public:
    TreeNode(int val = 0, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val_(val), leftNode_(l), rightNode_(r)
    {

    }
    int val_;
    TreeNode* leftNode_;
    TreeNode* rightNode_;
};

TreeNode* lowestCommonAncestor(
    TreeNode* root,
    TreeNode* p,
    TreeNode* q)
{
    if (root == nullptr) return nullptr;

    TreeNode* leftNode = lowestCommonAncestor(root->leftNode_, p, q);
    if (leftNode) return leftNode;
    TreeNode* rightNode = lowestCommonAncestor(root->rightNode_, p, q);
    if (rightNode) return rightNode;

    if (root->leftNode_ && root->rightNode_)
    {
        if (root->leftNode_ == p && root->rightNode_ == q)
        {
            return root;
        }
    }
    return nullptr;
}

int main()
{
    TreeNode* node1 = new TreeNode(4);
    TreeNode* node2 = new TreeNode(5);
    TreeNode* node3 = new TreeNode(2, node1, node2);
    TreeNode* node4 = new TreeNode(3);
    TreeNode* node5 = new TreeNode(1, node3, node4);
    TreeNode* node6 = new TreeNode(6);

    TreeNode* node = lowestCommonAncestor(node3, node1, node2);
    if (node) cout << node->val_ << endl;

    delete node1, node2, node3, node4, node5, node6;

    return 0;
}