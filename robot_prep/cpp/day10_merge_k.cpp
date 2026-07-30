#include <iostream>
#include <vector>
#include <memory>
#include <queue>

using namespace std;

class Node{
public:
    Node(int val = 0, shared_ptr<Node> next = nullptr)
        : val_(val), next_(next)
        {}
    int val_;
    shared_ptr<Node> next_;
    bool operator<(shared_ptr<Node> node)
    {
        return this->val_ < node->val_;
    }
};

struct cmp
{
    bool operator()(shared_ptr<Node> node1, shared_ptr<Node> node2)
    {
        return node1->val_ > node2->val_;
    }
};

shared_ptr<Node> getMergeNode(shared_ptr<Node> node1, shared_ptr<Node> node2, shared_ptr<Node> node3)
{
    priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, cmp> q;
    shared_ptr<Node> dummy = make_shared<Node>();
    shared_ptr<Node> tail = dummy;
    if (node1) q.push(node1);
    if (node2) q.push(node2);
    if (node3) q.push(node3);
    while(!q.empty())
    {
        shared_ptr<Node> curr = q.top();
        q.pop();

        tail->next_ = curr;
        tail = curr;

        if (curr->next_) q.push(curr->next_);
    }
    tail->next_ = nullptr;
    return dummy->next_;
}

void printTreeNode(shared_ptr<Node> node)
{
    shared_ptr<Node> root = node;
    while(root)
    {
        cout << root->val_ << " ";
        root = root->next_;
    }cout << endl;
}

int main()
{
    shared_ptr<Node> node1 = make_shared<Node>(5);
    shared_ptr<Node> node2 = make_shared<Node>(4, node1);
    shared_ptr<Node> node3 = make_shared<Node>(1, node2);

    shared_ptr<Node> node4 = make_shared<Node>(4);
    shared_ptr<Node> node5 = make_shared<Node>(3, node4);
    shared_ptr<Node> node6 = make_shared<Node>(1, node5);

    shared_ptr<Node> node7 = make_shared<Node>(6);
    shared_ptr<Node> node8 = make_shared<Node>(2, node7);

    printTreeNode(getMergeNode(node3, node6, node8));
    
    return 0;
}