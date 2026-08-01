#include <iostream>

#include "pluginlib/class_list_macros.hpp"
#include "robot_control_ros2/printer_base.hpp"

namespace robot_control_ros2
{

// 第一个具体插件：继承统一基类并实现纯虚函数。
class ChinesePrinter : public PrinterBase
{
public:
  void print() const override
  {
    std::cout << "你好，我是运行时加载的中文打印插件！\n";
  }
};

// 第二个插件用于演示：不修改加载程序，只改变插件名称即可切换实现。
class EnglishPrinter : public PrinterBase
{
public:
  void print() const override
  {
    std::cout << "Hello, I am a dynamically loaded printer plugin!\n";
  }
};

}  // namespace robot_control_ros2

// 将派生类及其基类类型注册给 pluginlib。它们随后会被编译进同一个 .so 文件。
PLUGINLIB_EXPORT_CLASS(robot_control_ros2::ChinesePrinter, robot_control_ros2::PrinterBase)
PLUGINLIB_EXPORT_CLASS(robot_control_ros2::EnglishPrinter, robot_control_ros2::PrinterBase)
