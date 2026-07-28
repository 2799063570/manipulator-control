#include <iostream>
#include <queue>
#include <stack>

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

void printTreeFront(TreeNode* head)
{
    if (head == nullptr) return;
    cout << head->val_ << " ";
    printTreeFront(head->leftNode_);
    printTreeFront(head->rightNode_);
}
void printTreeFrontStack(TreeNode* root)
{
    if (!root) return;
    stack<TreeNode*> treeStack;
    treeStack.push(root);
    while(!treeStack.empty())
    {
        TreeNode* stackTop = treeStack.top();
        treeStack.pop();
        cout << stackTop->val_  << " ";
        if (stackTop->rightNode_) treeStack.push(stackTop->rightNode_);
        if (stackTop->leftNode_) treeStack.push(stackTop->leftNode_);
    }cout << endl;
}
void printTreeMid(TreeNode* head)
{
    if (head == nullptr) return;
    printTreeMid(head->leftNode_);
    cout << head->val_ << " ";
    printTreeMid(head->rightNode_);
}
void printTreeLevel(TreeNode* head)
{
    queue<TreeNode*> treeQueue;
    treeQueue.push(head);
    while(!treeQueue.empty())
    {
        TreeNode* topNode = treeQueue.front();
        cout << topNode->val_ << " ";
        treeQueue.pop();
        if (topNode->leftNode_) treeQueue.push(topNode->leftNode_);
        if (topNode->rightNode_) treeQueue.push(topNode->rightNode_);
    }
    cout << endl;
}
int main()
{
    TreeNode* node1 = new TreeNode(4);
    TreeNode* node2 = new TreeNode(5);
    TreeNode* node3 = new TreeNode(2, node1, node2);
    TreeNode* node4 = new TreeNode(3);
    TreeNode* node5 = new TreeNode(1, node3, node4);
    printTreeFrontStack(node5);//cout << endl;
    return 0;
}