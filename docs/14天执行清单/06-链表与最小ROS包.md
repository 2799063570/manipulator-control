---
title: Day 06 - 链表与最小 ROS 包
tags: [14天打卡, 链表, ROS2]
---

# Day 06：链表与最小 ROS 包

关联计划：[[33-基础补齐-14天执行清单]]

## 当日任务

- 算法：反转链表、链表是否有环。
- C++：构造/析构、对象生命周期、RAII。
- OS：虚拟内存；栈与堆的角色。
- ROS：创建自己的最小 C++ package，包含 publisher 与 subscriber，并用 `colcon build` 构建。

- 算法：反转链表、链表是否有环。
## 反转链表（LeetCode 206）

### 描述

给定一个单链表的头节点 `head`，请你反转链表，并返回新的头节点。

例如：

输入：

```
1 -> 2 -> 3 -> 4 -> 5 -> nullptr
```

输出：

```
5 -> 4 -> 3 -> 2 -> 1 -> nullptr
```

```cpp
struct ListNode
{
    int val_;
    ListNode* nextNode_;
    ListNode(int val = 0, ListNode* next = NULL) : val_(val), nextNode_(next) 
    {

    }
};
ListNode* reverseList(ListNode* prev, ListNode* curr)
{
    
    if (curr->nextNode_ != nullptr)
    {
        ListNode* nextNode = curr->nextNode_;
        curr->nextNode_ = prev;

        reverseList(curr, nextNode);
    }
    curr->nextNode_ = prev;
    return curr;   
}
```

这里prev代表已经反转好的前半部分，curr代表正在处理的节点
我们将当前节点的指针指向调换，然后递归下一个节点

这里出现了几个问题
- reverseList(curr, nextNode); 这里没有返回 只有在最后return
当我们反转1 2 3
	reverseList(nullptr, 1)
		reverseList(1, 2)
			reverseList(2, 3)

只有最底层返回了3指针，但是reverseList没有接收 这个返回值直接丢失了
到最后指针返回第一次调用的curr；
- 空指针风险
	没有curr的判空操作， 我们只有`curr->nextNode_ != nullptr`操作
	当curr为空的时候直接崩溃

- 重复修改next
递归回来之后还会调用`curr->nextNode_ = prev;`
这就是一个思想的误区，并不是底层的return就结束了，还会递归回去继续调用

完善版本
```cpp
ListNode* reverseList(ListNode* prev, ListNode* curr)
{
	if (curr == nullptr) return prev;
	ListNode* nextNode = curr->nextNode_;
	curr->nextNode_ = prev;
	return reverseList(prev, nextNode);
}
```

递归最重要的问题：什么时候不用递归？
1 -> 2 -> 3 -> nullptr
reverse( nullptr, 1 )	
	reverse( 1, 2 )
		reverse( 2, 3 )
			reverse( 3, nullptr )
```
prev = 3
curr = nullptr
```

现在是什么状态？
已经没有节点需要处理了。
所以：

```
if(curr == nullptr)
{
    return prev;
}
```

这就是递归出口。

## 第一步：定义函数意义

不要写代码。
例如：二叉树：
```
int depth(TreeNode* root)
```
问：这个函数表示什么？
答案：返回root节点为根的树高度

链表：
```
reverse(head)
```
表示：返回head开始链表反转后的头节点
## 第二步：找最简单情况

问：什么时候问题不用继续拆？

比如链表：

### 空链表

```
nullptr
```

不用反转 返回：nullptr

所以：if(head==nullptr)

比如树：

```
depth(nullptr)
```

没有节点：高度：0

---

## 第三步：假设递归已经完成

这是递归最重要思想：

> 相信下一层递归是正确的。

例如：1 -> 2 -> 3

你现在处理：1
不要想着整个链表。
假设：
```
reverse(2)
```

已经成功：
得到：
```
3 -> 2
```

那么你只需要：

把1接到后面：

```
3 -> 2 -> 1
```

### 递归分析
反转链表
```cpp
ListNode* reverseList(ListNode* prev, ListNode* curr)
{
	if (curr == nullptr)
		return prev;
	ListNode* nextNode = curr->nextNode_;
	curr->nextNode_ = prev;
	return reverseList(curr, nextNode);
}
```
现在我要改变思路，处理当前节点的任务
相信后面的节点已经完成反转
把 curr 开始的链表反转，并且让反转后的链表接在 prev 后面，最后返回新的头节点。
所以我们要返回prev 代表新节点（任务实现返回最后的头节点）
```cpp
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
```
还是这个思路 我们相信 下面的节点都完成反转了 
所以我们只需要反转当前节点

```cpp
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
```
删除节点
还是 我们相信后面的节点 相信后面的节点已经完成了删除
因此我们只需判断当前节点是否需要删除
需要删除 我们只需要返回下一个节点
不需要返回返回当前节点
需要注意的是
我们调用当前函数 做递归返回的是处理好的头节点
```cpp
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
```
合并两个



- C++：构造/析构、对象生命周期、RAII。
- OS：虚拟内存；栈与堆的角色。
- ROS：创建自己的最小 C++ package，包含 publisher 与 subscriber，并用 `colcon build` 构建。

## 算法记录

- 日期：
- 分类：哈希 / 双指针 / 二分 / 栈队列 / 链表 / 树图 / 堆 / DP
- 限时：__ 分钟；实际：__ 分钟；是否独立完成：是 / 否
- 最终复杂度：时间 __；空间 __
- 边界测试：空输入 / 单元素 / 重复值 / 极值 / 其他
- 错误类型：知识缺口 / 思路 / 实现 bug / 边界 / 时间管理
- 一句话根因：
- 二刷日期：明天、7 天后

## C++ / OS 记录

- 今天理解的一点：
- RAII 最小例子：
- 栈与堆的区别：

## ROS / 项目记录

- package 名称：
- 构建与 source 命令：
- 两个节点的运行命令：
- 遇到的问题与下一步：

