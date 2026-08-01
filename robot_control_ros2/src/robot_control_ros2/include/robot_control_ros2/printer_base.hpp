#pragma once

namespace robot_control_ros2
{

/**
 * @brief 所有打印插件都必须实现的统一接口。
 *
 * 它相当于 AUBO 示例中的 hardware_interface::SystemInterface：加载程序只认识
 * PrinterBase，不需要在编译时知道 ChinesePrinter、EnglishPrinter 等派生类。
 */
class PrinterBase
{
public:
  // 通过基类指针删除派生对象时，基类析构函数必须是 virtual。
  virtual ~PrinterBase() = default;

  // 纯虚函数规定插件必须提供 print()，具体打印内容由派生类决定。
  virtual void print() const = 0;
};

}  // namespace robot_control_ros2
