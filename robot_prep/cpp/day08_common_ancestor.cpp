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

TreeNode* searchNode(
    TreeNode* root,
    int val)
{
    if (root == nullptr || root->val_ == val) return root;
    if (root->val_ > val) return searchNode(root->leftNode_, val);
    if (root->val_ < val) return searchNode(root->rightNode_, val);
}   

int main()
{
    TreeNode* node1 = new TreeNode(1);
    TreeNode* node2 = new TreeNode(6);
    TreeNode* node3 = new TreeNode(3, node1, node2);
    TreeNode* node4 = new TreeNode(14);
    TreeNode* node5 = new TreeNode(10, nullptr, node4);
    TreeNode* node6 = new TreeNode(8, node3, node5);

    TreeNode* getNode = searchNode(node6, 3);
    if (!getNode) cout << getNode->val_ << endl;

    delete node1, node2, node3, node4, node5, node6;

    return 0;
}