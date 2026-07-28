#include <iostream>
#include <queue>

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

int getHeight(TreeNode* root)
{
    if (root == nullptr) return 0;
    // 返回左树是否平衡 若平衡返回层数
    int leftDepth = getHeight(root->leftNode_);
    if (leftDepth == -1) return -1;
    int rightDepth = getHeight(root->rightNode_);
    if (rightDepth == -1) return -1;
    if (abs(rightDepth - leftDepth) > 1) return -1;
    return max(rightDepth, leftDepth) + 1;
}
bool isBalanceTree(TreeNode* root)
{
    return (getHeight(root) != -1);
}

int main()
{
    TreeNode* node1 = new TreeNode(4);
    TreeNode* node2 = new TreeNode(5);
    TreeNode* node3 = new TreeNode(2, node1, node2);
    TreeNode* node4 = new TreeNode(3);
    TreeNode* node5 = new TreeNode(1, node3, node4);
    TreeNode* node6 = new TreeNode(6);
    node1->leftNode_ = node6;

    cout << "is balance tree : " << (isBalanceTree(node5) ? "yes" : "no") << endl;

    delete node1;
    delete node2;
    delete node3;
    delete node4;
    delete node5;
    delete node6;
    
    return 0;
}