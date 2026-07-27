#include <iostream>


using namespace std;

struct ListNode
{
    int val_;
    ListNode* nextNode_;
    ListNode(int val = 0, ListNode* next = NULL) : val_(val), nextNode_(next) 
    {

    }
};


// 反转链表
ListNode* reverseList(ListNode* prev, ListNode* curr)
{
    
    if (curr == nullptr) return prev;
    ListNode* nextNode = curr->nextNode_;
    curr->nextNode_ = prev;
    return reverseList(curr, nextNode);
}
// 反转链表
ListNode* reverseList(ListNode* head)
{
    if (head == nullptr || head->nextNode_ == nullptr)
    {
        return head;
    }
    ListNode* nextNode = head->nextNode_;
    nextNode->nextNode_ = head;
    return reverseList(nextNode);
}
// 删除节点
ListNode* delListVal(ListNode* head, int val)
{
    if (head == nullptr) return nullptr;

    if (head->val_ == val) return delListVal(head->nextNode_, val);
    else 
    {
        head->nextNode_ = delListVal(head->nextNode_, val);
        return head;
    }
}
// 合并两个链表 按照从小到大 顺序
ListNode* mergeList(ListNode* head1, ListNode* head2)
{
    if (head1 == nullptr)
    {
        if (head2 != nullptr)
            return head2;
        else return nullptr;
    }
    if (head2 == nullptr)
    {
        if (head1 != nullptr)
            return head1;
        else return nullptr;
    }
    if (head1->val_ <= head2->val_)
    {
        head1->nextNode_ = mergeList(head1->nextNode_, head2);
        return head1;
    }else
    {
        head2->nextNode_ = mergeList(head1, head2->nextNode_);
        return head2;
    }
}

ListNode* reverseListSection(ListNode* head, int l, int r, int count)
{
    if (head == nullptr) return nullptr;
    if (count < l)
    {
        head = reverseListSection(head->nextNode_, l, r, ++count);
        return head;
    }
    if (count >= l && count <=r)
    {
        if (head->nextNode_ == nullptr || count == r)
            return head;
        else
        {
            ListNode* nextNode = head->nextNode_;
            nextNode->nextNode_ = head;
            return reverseList(nextNode);
        }
    }
    if (count > r)
        return head;
}
void printList(ListNode* head)
{
    ListNode* printNode = head;
    while (printNode != nullptr)
    {
        cout << printNode->val_ << " ";
        printNode = printNode->nextNode_;
    }cout << endl;
}
int main()
{
    ListNode* l1 = new ListNode(4, nullptr);
    ListNode* l2 = new ListNode(2, l1);
    ListNode* l3 = new ListNode(2, l2);
    ListNode* l4 = new ListNode(1, l3);

    ListNode* l5 = new ListNode(5, nullptr);
    ListNode* l6 = new ListNode(3, l5);
    ListNode* l7 = new ListNode(2, l6);
    ListNode* l8 = new ListNode(1, l7);

    printList(l4);
    
    // printList(reverseList(l4));
    // printList(delListVal(l4, 2));
    printList(mergeList(l4, l8));
    // cout << (l4->nextNode_ == nullptr ? "null" : "no nullptr") << endl;
    // printList(reverseList(nullptr, l4));
    // cout << reverseList(nullptr, l4)->val_ <<endl;
    // cout << (l4->nextNode_ == nullptr ? "null" : "no nullptr") << endl;
    // printList(l1);

    return 0;
}