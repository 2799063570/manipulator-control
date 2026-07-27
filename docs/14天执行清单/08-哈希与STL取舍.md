---
title: Day 08 - 哈希与 STL 取舍
tags: [14天打卡, 哈希, STL]
---

# Day 08：哈希与 STL 取舍

关联计划：[[33-基础补齐-14天执行清单]]

## 当日任务

- 算法：两数之和二刷、字母异位词分组。
- C++：理解 `map` 与 `unordered_map` 的有序性、复杂度、冲突与选型。
- ROS：使用 `ros2 topic info -v <topic>` 查看发布者、订阅者和 QoS。

## 题目一：两数之和（二刷）

给定整数数组 `nums` 和目标值 `target`，找出和为 `target` 的两个元素，并返回它们的下标。每个输入只对应一个答案，且同一个元素不能重复使用。

### 复杂度与思路

- 暴力枚举所有元素对：时间复杂度 O(n²)，额外空间复杂度 O(1)。
- 哈希表：遍历到 `nums[i]` 时，查询补数 `target - nums[i]` 是否已出现。平均时间复杂度 O(n)，额外空间复杂度 O(n)。

```cpp
vector<int> getTwoSumIndices(const vector<int>& nums, int target)
{
    unordered_map<int, int> indexByValue;

    for (int i = 0; i < static_cast<int>(nums.size()); ++i)
    {
        const int complement = target - nums[i];
        auto it = indexByValue.find(complement);
        if (it != indexByValue.end())
            return {it->second, i};

        indexByValue[nums[i]] = i;
    }
    return {};  // 若题目保证存在答案，这一分支不会触发
}
```

本题只需要快速判断补数是否出现过，并不需要按键排序，因此 `unordered_map` 通常更合适。它的查找、插入平均为 O(1)；`map` 通常基于平衡二叉搜索树，查找、插入和删除为 O(log n)，但能保持键的有序性。

### 复盘检查

- 先查询补数，再写入当前元素，才能避免同一个元素被重复使用。
- 无解时返回 `{}` 比返回 `{0, 0}` 更通用；`{0, 0}` 容易被误解为有效下标答案。
- 变量名不要写成 `map`，避免遮蔽 `std::map` 类型名。

## 题目二：字母异位词分组

给定字符串数组：

```text
strs = ["eat", "tea", "tan", "ate", "nat", "bat"]
```

将由相同字符、相同次数组成的字符串分到同一组。例如 `"eat"`、`"tea"` 和 `"ate"` 排序后都为 `"aet"`，可使用这个排序结果作为哈希键。

```cpp
vector<vector<string>> groupAnagrams(const vector<string>& strs)
{
    unordered_map<string, vector<string>> groups;

    for (const string& str : strs)
    {
        string key = str;
        sort(key.begin(), key.end());
        groups[key].push_back(str);
    }

    vector<vector<string>> result;
    result.reserve(groups.size());
    for (const auto& [key, group] : groups)
        result.push_back(group);

    return result;
}
```

设字符串数量为 `n`，单个字符串平均长度为 `k`。排序键的总时间复杂度为 O(n · k log k)，额外空间复杂度为 O(n · k)。由于使用 `unordered_map`，不同分组在返回结果中的先后顺序不保证；若题目要求稳定顺序，需要额外排序或改用有序容器。

## C++：`map` 与 `unordered_map` 如何选择

| 对比项 | `map` | `unordered_map` |
| --- | --- | --- |
| 键的顺序 | 按比较器有序 | 无固定顺序 |
| 常见底层结构 | 平衡搜索树 | 哈希表与桶 |
| 查找/插入/删除 | O(log n) | 平均 O(1)，最坏 O(n) |
| 范围查询、按序遍历 | 适合 | 不适合 |
| 迭代器失效 | 插入通常不影响既有迭代器；删除仅影响被删元素 | rehash 可能使迭代器失效 |
| 典型用途 | 排名、区间查询、按键有序输出 | 频繁按键查找、计数、去重 |

### 哈希冲突与 rehash

哈希冲突指不同的键被分配到同一个桶。发生冲突并不代表容器错误，容器会在桶内继续比较键来区分元素；但冲突过多会使查找性能下降。

`unordered_map` 会根据负载因子（元素数与桶数的比例）在适当时机扩容并 rehash，以维持平均查找效率。若能估计元素数量，可提前调用 `reserve()`，减少构建过程中多次 rehash：

```cpp
unordered_map<int, int> indexByValue;
indexByValue.reserve(nums.size());
```

不要简单认为哪种容器“占用一定更大”：`map` 的每个节点有额外树结构开销，`unordered_map` 也需要桶数组并可能保留空桶。实际内存占用取决于元素数量、键值类型、负载因子和标准库实现。

## ROS2：使用 `ros2 topic info -v` 查看 QoS

基础命令可以查看话题类型，以及发布者和订阅者数量：

```bash
ros2 topic info /turtle1/cmd_vel
```

```text
Type: geometry_msgs/msg/Twist
Publisher count: 1
Subscription count: 1
```

加入 `-v` 后会显示每个端点所属节点、端点类型以及 QoS Profile：

```bash
ros2 topic info -v /turtle1/cmd_vel
```

输出中的核心字段包括：

- `Reliability`：`RELIABLE` 尽量可靠传输；`BEST_EFFORT` 允许丢包以减少等待和重传。
- `History` 与 `Depth`：决定保留多少条历史消息；`KEEP_LAST` 搭配深度最常见。
- `Durability`：`VOLATILE` 不为后来订阅者保留旧消息；`TRANSIENT_LOCAL` 可由仍在线的发布者向后来订阅者提供缓存消息。
- `Deadline`、`Lifespan`、`Liveliness`：分别用于期望发布周期、消息有效期与端点活性检测。

### ROS1、ROS2 与 QoS：复盘要点

- ROS1 常见传输协议是 TCPROS，也支持 UDPROS；不能简单概括为“ROS1 只采用 TCP”。
- ROS2 通常基于 DDS 中间件，支持自动发现和发布—订阅解耦；具体数据传输仍取决于所选 DDS 实现和配置。
- QoS 要根据数据特性选择，而不是固定规定“控制指令必须可靠、图像必须尽力而为”。例如对偶尔丢失可接受、只关心最新状态的高频传感器数据，`BEST_EFFORT` 往往合适；必须送达的配置或任务结果，`RELIABLE` 往往更合适。
- QoS 不兼容会导致发布者和订阅者无法匹配。排障时先运行 `ros2 topic info -v <topic>`，比较双方的可靠性、持久性和历史设置。

## 算法记录

- 日期：
- 分类：哈希 / 双指针 / 二分 / 栈队列 / 链表 / 树图 / 堆 / DP
- 限时：__ 分钟；实际：__ 分钟；是否独立完成：是 / 否
- 最终复杂度：时间 __；空间 __
- 边界测试：空输入 / 单元素 / 重复值 / 极值 / 其他
- 错误类型：知识缺口 / 思路 / 实现 bug / 边界 / 时间管理
- 一句话根因：
- 二刷日期：明天、7 天后

## C++ / ROS 记录

- 今天理解的一点：
- `map` 与 `unordered_map` 的选择：
- topic、类型、发布者、订阅者、QoS：
- 遇到的问题与下一步：
