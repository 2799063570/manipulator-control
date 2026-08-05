#include <iostream>
#include <string>

#include "pluginlib/class_loader.hpp"
#include "pluginlib/exceptions.hpp"
#include "robot_control_ros2/printer_base.hpp"

int main(int argc, char ** argv)
{
  // 不传参数时加载中文插件，也可以在运行时传入另一个 XML 中注册的插件名。 test
  const std::string plugin_name = argc > 1 ? argv[1] : "robot_control_ros2/ChinesePrinter";

  try
  {
    // 第一个参数是插件所在 ROS 包，第二个参数是插件共同继承的 C++ 基类类型。
    pluginlib::ClassLoader<robot_control_ros2::PrinterBase> loader(
      "robot_control_ros2", "robot_control_ros2::PrinterBase");

    std::cout << "准备加载插件：" << plugin_name << '\n';

    // pluginlib 根据插件名查 XML，再通过 dlopen() 加载 .so，最后创建派生类对象。
    // 主程序没有直接链接 printer_plugins，因此这里才是插件真正被装入的时刻。
    auto printer = loader.createSharedInstance(plugin_name);

    // 虽然变量类型是 PrinterBase 指针，虚函数仍会调用实际派生类的实现。
    printer->print();
    std::cout << "插件调用完成。\n";
    return 0;
  }
  catch (const pluginlib::PluginlibException & exception)
  {
    std::cerr << "插件加载失败：" << exception.what() << '\n';
    return 1;
  }
}
