#include <iostream>
#include <stack>
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

int getTreeMaxDepth(TreeNode* root)
{
    if (!root) return 1;
    int leftDepth = getTreeMaxDepth(root->leftNode_);
    int rightDepth = getTreeMaxDepth(root->rightNode_);
    return max(leftDepth, rightDepth) + 1;
}
int getTreeMaxDepthStack(TreeNode* root)
{
    if (!root) return 1;
    queue<TreeNode*> treeQueue;
    treeQueue.push(root);
    int count = 1;
    while(!treeQueue.empty())
    {
        int size = treeQueue.size();
        for (int i = 0; i<size; ++i)
        {
            TreeNode* topNode = treeQueue.front();
            treeQueue.pop();
            if (topNode->leftNode_) treeQueue.push(topNode->leftNode_);
            if (topNode->rightNode_) treeQueue.push(topNode->rightNode_);
        }count++;
    }
    return count;
}
int main()
{
    TreeNode* node1 = new TreeNode(4);
    TreeNode* node2 = new TreeNode(5);
    TreeNode* node3 = new TreeNode(2, node1, node2);
    TreeNode* node4 = new TreeNode(3);
    TreeNode* node5 = new TreeNode(1, node3, node4);

    cout << "get Tree depth : " << getTreeMaxDepthStack(node5) << endl;
    return 0;
}