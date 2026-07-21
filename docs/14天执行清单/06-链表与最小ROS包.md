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

### 错误记录（保留）：第一版递归写法

下面的原始实现保留用于复盘。它缺少 `curr == nullptr` 的出口，且递归调用的返回值没有被正确传递；回溯时又重复修改 `nextNode_`，因此不能作为正确答案。

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

### 复盘：问题、触发条件与根因

- 空链表反例：`reverseList(nullptr, nullptr)` 会在访问 `curr->nextNode_` 时解引用空指针。
- 返回值丢失：`reverseList(curr, nextNode)` 的结果没有向上传递，最深层得到的新头节点无法成为第一层调用的返回值。
- 回溯时重复改链：递归调用结束后，上一层仍会执行 `curr->nextNode_ = prev`，这不是“递归结束”，而是开始逐层回溯，容易破坏已经建立的链接。

### 错误记录（保留）：传参方向错误的“完善版本”

以下代码虽然增加了空指针出口，但递归调用写成了 `reverseList(prev, nextNode)`。当前节点已经指向 `prev`，下一层应以当前节点 `curr` 作为新的已反转部分；传入旧的 `prev` 会丢失当前节点。

```cpp
ListNode* reverseList(ListNode* prev, ListNode* curr)
{
	if (curr == nullptr) return prev;
	ListNode* nextNode = curr->nextNode_;
	curr->nextNode_ = prev;
	return reverseList(prev, nextNode);
}
```

最小反例：`1 -> 2 -> nullptr`。第二次调用仍传入 `prev = nullptr`，节点 `1` 无法被正确接入已反转部分。

### 正确实现：尾递归风格

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

`prev` 表示已经反转好的前半段，`curr` 表示当前待处理节点。递归出口为“当前没有节点可处理”，此时 `prev` 就是新的头节点。时间复杂度为 O(n)，递归调用栈空间为 O(n)。

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

所以：`if(head==nullptr)`

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

### 另一种递归定义：返回反转后的新头节点

函数 `reverseList(head)` 的含义是：返回从 `head` 开始的链表反转后的新头节点。假设 `reverseList(head->nextNode_)` 已经正确完成后半段反转，当前层只需把 `head` 接到反转链表的尾部，并断开原来的正向链接。

### 错误记录（保留）：未断开旧链接

下面的原始实现少了 `head->nextNode_ = nullptr`，会保留 `head -> nextNode` 的旧链接，从而形成环；同时它没有保存并返回递归得到的新头节点。

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

最小反例：`1 -> 2 -> nullptr`。执行后同时存在 `1 -> 2` 和 `2 -> 1`，链表变成环。

### 复盘与正确实现

```cpp
ListNode* reverseListRecursive(ListNode* head)
{
    if (head == nullptr || head->nextNode_ == nullptr)
        return head;

    ListNode* newHead = reverseListRecursive(head->nextNode_);
    head->nextNode_->nextNode_ = head;
    head->nextNode_ = nullptr;
    return newHead;
}
```

这个版本的关键是：先让后半段完成反转并保存新头节点；再把当前节点接到原后继节点之后；最后断开当前节点的旧链接。

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

删除节点同样采用“相信后续递归已完成”的思路：当前节点需要删除时，返回已处理的后继节点；否则将 `head->nextNode_` 接到递归结果并返回当前节点。若节点由当前程序用 `new` 创建并由本函数负责释放，还应在返回前执行 `delete head`；在线上题平台中，是否需要释放节点应以题目约定为准。
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
合并两个有序链表时，函数的含义是“返回两个当前头节点合并后的新头节点”。当任一链表为空时，另一个链表就是结果；否则选择较小节点作为当前头节点，并将其 `nextNode_` 指向剩余部分的递归合并结果。时间复杂度为 O(m + n)，递归调用栈空间为 O(m + n)。

## 链表是否有环：Floyd 快慢指针

原任务包含“链表是否有环”，但原稿没有展开实现。补充一个最小复习版本：慢指针每次走一步，快指针每次走两步；若链表有环，二者一定会在环内相遇；若快指针先到达 `nullptr`，则链表无环。

```cpp
bool hasCycle(ListNode* head)
{
    ListNode* slow = head;
    ListNode* fast = head;

    while (fast != nullptr && fast->nextNode_ != nullptr)
    {
        slow = slow->nextNode_;
        fast = fast->nextNode_->nextNode_;
        if (slow == fast)
            return true;
    }
    return false;
}
```

复盘测试：空链表、单节点无环、单节点自环、两节点成环。时间复杂度为 O(n)，额外空间复杂度为 O(1)。


- C++：构造/析构、对象生命周期、RAII。

对象生命周期是对象从创建、初始化、使用到销毁的完整过程。
## Q1：构造函数什么时候调用？

答：

> 对象创建时自动调用，用于初始化对象。

---

## Q2：析构函数什么时候调用？

答：

> 对象生命周期结束时自动调用，用于释放对象持有的资源。

---

## Q3：RAII是什么？

答：

> 将资源生命周期绑定到对象生命周期，通过构造获取资源，通过析构释放资源。

---

## Q4：为什么C++推荐智能指针？

答：

> 利用RAII自动管理内存，避免忘记delete导致内存泄漏。

---

## Q5：ROS2为什么大量使用shared_ptr？

答：

> ROS2 接口经常以 `shared_ptr` 形式暴露，便于多个组件或回调安全持有同一对象。它表示共享所有权；并非所有对象都必须使用 `shared_ptr`，应避免不必要的共享与循环引用。

- OS：虚拟内存；栈与堆的角色。

## Q1：什么是虚拟内存？

答：

> 操作系统为每个进程提供独立的虚拟地址空间，并通过页表将虚拟地址映射到物理内存或其他后备存储，从而实现隔离、按需分配和统一的地址视图。

---

## Q2：为什么需要虚拟内存？

三个原因：

```
1. 进程隔离：一个进程不能直接读写另一个进程的内存。

2. 按需分配与页面换入换出：不必一次性将所有页面放入物理内存。

3. 连续的虚拟地址视图：程序可使用连续的地址空间，而底层物理页不必连续。
```

---

## Q3：栈和堆区别？

答：

```
栈：
函数调用帧和自动存储期的局部对象通常位于栈上；离开作用域时自动销毁，容量通常较小。

堆（自由存储区）：
动态分配的对象通常位于堆上；可由 `unique_ptr`、`shared_ptr` 等 RAII 对象自动管理，不应简单理解为“必须手动管理”。
```

---

## Q4：为什么shared_ptr管理堆对象？

答：

> `shared_ptr` 通过引用计数管理其所拥有的动态对象；最后一个共享所有者释放后，对象会被销毁。需要注意循环引用会使引用计数无法归零，此时可用 `weak_ptr` 打破环。
- ROS：创建自己的最小 C++ package，包含 publisher 与 subscriber，并用 `colcon build` 构建。

## ROS2：最小 C++ package

### 错误记录（保留）

原始命令中的构建类型拼成了 `ament_cmke`。正确名称是 `ament_cmake`；该错误会导致功能包创建失败。

```bash
# 原始错误：--build-type ament_cmke
ros2 pkg create my_minimal_cpp_pkg --build-type ament_cmake --dependencies rclcpp std_msgs
```

在工作区根目录构建并加载当前终端环境：

```bash
colcon build --packages-select my_minimal_cpp_pkg
source install/setup.bash
```

一个最小发布者/订阅者包通常至少包含：

- `package.xml`：声明 `rclcpp`、消息包等依赖；
- `CMakeLists.txt`：构建可执行文件并声明依赖；
- `src/publisher.cpp` 与 `src/subscriber.cpp`：分别创建发布者和订阅者；
- 构建后使用 `ros2 run <package> <executable>` 启动节点。

### 项目控制节点阅读记录（不是最小包）

下面代码保留为现有机械臂控制节点的阅读记录。它实现的是“订阅速度指令 → 生成并缓存轨迹 → 定时发布位置指令”的控制链路，依赖自定义消息和多个项目类；它比最小 publisher/subscriber 包复杂得多。

```cpp
#include "rclcpp/rclcpp.hpp"

#include "robot_control_ros2/msg/joint_velocity_cmd.hpp"

#include "robot_control_ros2/msg/joint_position_cmd.hpp"

#include "robot_state.hpp"

#include "safety_limiter.hpp"

#include "cubic_interpolator.hpp"

#include "trajectory_buffer.hpp"

#include <vector>



using JointVelocityCmd = robot_control_ros2::msg::JointVelocityCmd;

using JointPositionCmd = robot_control_ros2::msg::JointPositionCmd;



class Vel2PosNode : public rclcpp::Node

{

    public:

        Vel2PosNode() : Node("vel_to_pos_node"),

            dof_(this->declare_parameter<int>("dof", 6)),

            max_velocity_(this->declare_parameter<double>("max_velocity", 1.2)),

            duration_(this->declare_parameter<double>("velocity_cmd_time", 0.1)),

            control_rate_(this->declare_parameter<double>("control_rate", 50)),

            control_time_(1000/control_rate_),

            dt_(this->declare_parameter<double>("dt", control_time_*0.001)),

            robot_state_start_(dof_), robot_state_goal_(dof_),

            safety_limiter_(dof_, max_velocity_),

            trajectory_buffer_(dof_),

            cubic_interpolator_(dof_, duration_, dt_)

        {



            std::vector<double> lower_limits_ = this->declare_parameter<std::vector<double>>(

                "lower_limits",

                std::vector<double>(dof_, -3.14)

            );

            std::vector<double> upper_limits_ = this->declare_parameter<std::vector<double>>(

                "upper_limits",

                std::vector<double>(dof_, 3.14)

            );

            safety_limiter_.setLowerLimits(lower_limits_);

            safety_limiter_.setUpperLimits(upper_limits_);



            robot_state_start_.setPosition(std::vector<double>(dof_, double{}));

            robot_state_start_.setPosition(std::vector<double>(dof_, double()));

            robot_state_goal_.setPosition(std::vector<double>(dof_, double{}));

            robot_state_goal_.setPosition(std::vector<double>(dof_, double()));



            trajectory_buffer_.clear();



            sub_ = this->create_subscription<JointVelocityCmd>("joint_velocity_cmd", 10,

                std::bind(&Vel2PosNode::sub_callback, this, std::placeholders::_1));

            pub_ = this->create_publisher<JointPositionCmd>("joint_position_cmd", 10);



            timer_ = this->create_wall_timer(std::chrono::milliseconds(control_time_),

                std::bind(&Vel2PosNode::timer_callback, this));

            RCLCPP_INFO(this->get_logger(), "Finish node init...");

        }

    private:

        void sub_callback(const JointVelocityCmd& vel)

        {

            std::vector<double> robot_velocity;

            std::vector<double> robot_position;

            for (int i = 0; i<dof_; i++)

            {

                robot_velocity.push_back(vel.velocity_cmd[i]);

                robot_position.push_back(robot_state_goal_.position()[i] + (vel.velocity_cmd[i] + robot_state_goal_.velocity()[i])*0.5*duration_);

            }

            robot_state_goal_.setVelocity(robot_velocity);

            robot_state_goal_.setPosition(robot_position);

            safety_limiter_.clampRobotState(robot_state_goal_);// 位置速度限幅

            RCLCPP_INFO(this->get_logger(), "clamp velocity cmd: [%.2f, %.2f, %.2f, %.2f, %.2f, %.2f]",

                robot_state_goal_.velocity()[0], robot_state_goal_.velocity()[1], robot_state_goal_.velocity()[2],

                robot_state_goal_.velocity()[3], robot_state_goal_.velocity()[4], robot_state_goal_.velocity()[5]);

            RCLCPP_INFO(this->get_logger(), "clamp position cmd: [%.2f, %.2f, %.2f, %.2f, %.2f, %.2f]",

                robot_state_goal_.position()[0], robot_state_goal_.position()[1], robot_state_goal_.position()[2],

                robot_state_goal_.position()[3], robot_state_goal_.position()[4], robot_state_goal_.position()[5]);

            if (robot_state_goal_ != robot_state_start_)

            {

                cubic_interpolator_.setStateStart(robot_state_start_);

                cubic_interpolator_.setStateGoal(robot_state_goal_);

                cubic_interpolator_.calculate5Times();



                int trajectory_nums = cubic_interpolator_.getStateNum();

                std::vector<RobotState> trajectory = cubic_interpolator_.getTrajectory();

                for (int i  = 1; i < trajectory_nums; ++i)

                {

                    trajectory_buffer_.pushRobotState(trajectory[i]);

                }

            }

        }

        void timer_callback()

        {

            JointPositionCmd position_cmd;// 位置指令



            if (trajectory_buffer_.hasNext())

            {

                RobotState robotState = trajectory_buffer_.popNext();

                for (int i = 0; i<dof_; i++)

                {          

                    position_cmd.position_cmd[i] = robotState.position()[i];

                }

                RCLCPP_INFO(this->get_logger(), "pub [%.2f, %.2f, %.2f, %.2f, %.2f, %.2f] to position node",

                    robotState.position()[0], robotState.position()[1], robotState.position()[2],

                    robotState.position()[3], robotState.position()[4], robotState.position()[5]);

                pub_->publish(position_cmd);

                robot_state_start_ = robot_state_goal_;

            } else

            {

                RCLCPP_INFO(this->get_logger(), "deque has not robot states");

            }

        }



        rclcpp::TimerBase::SharedPtr timer_;

        rclcpp::Subscription<JointVelocityCmd>::SharedPtr sub_;

        rclcpp::Publisher<JointPositionCmd>::SharedPtr pub_;

        int dof_;

        double max_velocity_;

        double duration_;

        int control_rate_;

        int control_time_;      

        double dt_;

        RobotState robot_state_start_, robot_state_goal_;

        SafetyLimiter safety_limiter_;

        TrajectoryBuffer trajectory_buffer_;

        CubicInterpolator cubic_interpolator_;

};

int main(int argc, char* argv[])

{

    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<Vel2PosNode>());

    rclcpp::shutdown();

    return 0;

}
```

### 控制链路总结

该节点的流程是：订阅速度指令，计算目标关节状态并进行安全限幅；如状态发生变化，则生成三次插值轨迹并压入缓冲区；定时器按控制周期从缓冲区取出状态并发布位置指令。

```txt
joint_velocity_cmd
        |
        v
订阅回调：计算目标状态、限幅、生成轨迹
        |
        v
TrajectoryBuffer
        |
        v
定时器回调：按周期取轨迹点
        |
        v
joint_position_cmd
```

### 项目代码复盘点

- `setPosition(...)` 对起始状态和目标状态各调用了两次，属于重复初始化；保留原代码便于检查实际意图。
- `control_time_ = 1000 / control_rate_` 是整数除法。当频率不能整除 1000 时会丢失精度；频率大于 1000 时还可能得到 0。更稳妥的做法是使用 `std::chrono` 持有周期，或以 `double` 计算秒数。
- 日志固定访问关节下标 `0` 到 `5`，因此隐含 `dof_ == 6`。若允许参数修改自由度，应改为遍历打印，或验证自由度必须为 6。
- 回调和定时器都会访问控制状态。单线程执行器下回调不会并行；若改为多线程执行器，则需要评估 `robot_state_goal_`、`trajectory_buffer_` 等共享数据的同步策略。
- 使用自定义消息的 `position_cmd`、`velocity_cmd` 前，应确认其数组长度与 `dof_` 一致，避免按下标写入时越界。
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
