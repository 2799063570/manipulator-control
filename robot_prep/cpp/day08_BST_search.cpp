#include <iostream>
#include <stack>
#include <memory>

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
    else return searchNode(root->rightNode_, val);
}   
TreeNode* searchNodeStack(
    TreeNode* root, 
    int val)
{
    stack<TreeNode*> treeStack;
    treeStack.push(root);
    while(!treeStack.empty())
    {
        TreeNode* topNode = treeStack.top();
        treeStack.pop();
        if (topNode->val_ == val) return topNode;
        else if (topNode->val_ > val)
        {
            treeStack.push(topNode->leftNode_);
        }
        else treeStack.push(topNode->rightNode_);
    }
    return nullptr;
}

int main()
{
    TreeNode* node1 = new TreeNode(1);
    TreeNode* node2 = new TreeNode(6);
    TreeNode* node3 = new TreeNode(3, node1, node2);
    TreeNode* node4 = new TreeNode(14);
    TreeNode* node5 = new TreeNode(10, nullptr, node4);
    TreeNode* node6 = new TreeNode(8, node3, node5);

    TreeNode* getNode = searchNodeStack(node6, 10);
    if (getNode) cout << getNode->val_ << endl;

    delete node1;
    delete node2;
    delete node3;
    delete node4;
    delete node5;
    delete node6;

    shared_ptr<TreeNode> p = make_shared<TreeNode>(1);
    weak_ptr<TreeNode> p1 = p;
    cout << p.use_count() << endl;
    {
        auto p2 = p1.lock();
        if (p2)
        {
            cout << p2->val_ << endl;
            cout << p.use_count() << endl;
        }  
    }
    cout << p.use_count() << endl;

    return 0;
}