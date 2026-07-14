/** @file  register_control.h
 *  @brief 寄存器操作接口，用于三个模块之间的数据交换功能
 */
#ifndef AUBO_SDK_REGISTER_CONTROL_INTERFACE_H
#define AUBO_SDK_REGISTER_CONTROL_INTERFACE_H

#include <stdint.h>
#include <memory>
#include <vector>

#include <aubo/type_def.h>
#include <aubo/global_config.h>

enum ModbusErrorNum
{
    /** MODBUS unit not initiallized
     */
    MB_ERR_NOT_INIT = -1,

    /** MODBUS unit disconnected
     */
    MB_ERR_DISCONNECTED = -2,

    /** The function code received in the query is not an allowable action for
     * the server (or slave).
     */
    MB_ERR_ILLEGAL_FUNCTION = 1,

    /** The function code received in the query is not an allowable action for
     * the server (or slave), check that the entered signal address corresponds
     * to the setup of the remote MODBUS server.
     */
    MB_ERR_ILLEGAL_DATA_ADDRESS = 2,

    /** A value contained in the query data field is not an allowable value for
     * server (or slave), check that the enterd signal value is valid for the
     * specified address on the remote MODBUS server.
     */
    MB_ERR_ILLEGAL_DATA_VALUE = 3,

    /** An unrecoverable error occurred while the server (or slave) was
     * attempting to perform the requested action.
     */
    MB_ERR_SLAVE_DEVICE_FAILURE = 4,

    /** Specialized use in conjunction with programming commands sent to the
     * remote MODBUS unit.
     */
    MB_ERR_ACKNOWLEDGE = 5,

    /** Specialized use in conjunction with programming commands sent to the
     * remote MODBUS unit, the slave (server) is not able to respond now
     */
    MB_ERR_SLAVE_DEVICE_BUSY = 6,
};

namespace arcs {
namespace common_interface {

/**
 * 通用寄存器
 */
class ARCS_ABI_EXPORT RegisterControl
{
public:
    RegisterControl();
    virtual ~RegisterControl();

    /**
     * Reads the boolean from one of the input registers, which can also be
     * accessed by a Field bus. Note, uses it’s own memory space.
     *
     * 从一个输入寄存器中读取布尔值，也可以通过现场总线进行访问。
     * 注意，它使用自己的内存空间。
     *
     * @param address Address of the register (0:127)
     * 寄存器的地址（0:127）
     * @return The boolean value held by the register (true, false)
     * 寄存器中保存的布尔值（true、false）
     *
     * @note The lower range of the boolean input registers [0:63] is reserved
     * for FieldBus/PLC interface usage. The upper range [64:127] cannot be
     * accessed by FieldBus/PLC interfaces, since it is reserved for external
     * RTDE clients.
     * 布尔输入寄存器的较低范围[0:63]保留供FieldBus/PLC接口使用。
     * 较高范围[64:127]无法通过FieldBus/PLC接口访问，因为它保留供外部RTDE客户端使用。
     *
     * @par Python函数原型
     * getBoolInput(self: pyaubo_sdk.RegisterControl, arg0: int) -> bool
     *
     * @par Lua函数原型
     * getBoolInput(address: number) -> boolean
     *
     */
    bool getBoolInput(uint32_t address);

    /**
     *
     * @param address
     * @param value
     * @return
     *
     * @note 只有在实现 RTDE/Modbus Slave/PLC 服务端时使用
     *
     * @par Python函数原型
     * setBoolInput(self: pyaubo_sdk.RegisterControl, arg0: int, arg1: bool) ->
     * int
     *
     * @par Lua函数原型
     * setBoolInput(address: number, value: boolean) -> nil
     *
     */
    int setBoolInput(uint32_t address, bool value);

    /**
     * Reads the integer from one of the input registers, which can also be
     * accessed by a Field bus. Note, uses it’s own memory space.
     *
     * 从一个输入寄存器中读取整数值，也可以通过现场总线进行访问。
     * 注意，它使用自己的内存空间。
     *
     * @param address Address of the register (0:47)
     * 寄存器的地址（0:47）
     * @return The value held by the register [-2,147,483,648 : 2,147,483,647]
     * 寄存器中保存的整数值[-2,147,483,648 : 2,147,483,647]
     *
     * @note The lower range of the integer input registers [0:23] is reserved
     * for FieldBus/PLC interface usage. The upper range [24:47] cannot be
     * accessed by FieldBus/PLC interfaces, since it is reserved for external
     * RTDE clients.
     * 整数输入寄存器的较低范围[0:23]保留供FieldBus/PLC接口使用。
     * 较高范围[24:47]无法通过FieldBus/PLC接口访问，因为它保留供外部RTDE客户端使用。
     *
     * @par Python函数原型
     * getInt32Input(self: pyaubo_sdk.RegisterControl, arg0: int) -> int
     *
     * @par Lua函数原型
     * getInt32Input(address: number) -> number
     *
     */
    int getInt32Input(uint32_t address);

    /**
     *
     * @param address
     * @param value
     * @return
     *
     * @note 只有在实现 RTDE/Modbus Slave/PLC 服务端时使用
     *
     * @par Python函数原型
     * setInt32Input(self: pyaubo_sdk.RegisterControl, arg0: int, arg1: int) ->
     * int
     *
     * @par Lua函数原型
     * setInt32Input(address: number, value: number) -> nil
     *
     */
    int setInt32Input(uint32_t address, int value);

    /**
     * Reads the float from one of the input registers, which can also be
     * accessed by a Field bus. Note, uses it’s own memory space.
     *
     * 从一个输入寄存器中读取浮点数，也可以通过现场总线进行访问。
     * 注意，它使用自己的内存空间。
     *
     * @param address Address of the register (0:47)
     * 寄存器地址（0:47）
     * @return The value held by the register (float)
     * 寄存器中保存的浮点数值
     *
     * @note The lower range of the float input registers [0:23] is reserved
     * for FieldBus/PLC interface usage. The upper range [24:47] cannot be
     * accessed by FieldBus/PLC interfaces, since it is reserved for external
     * RTDE clients.
     * 浮点数输入寄存器的较低范围[0:23]保留供现场总线/PLC接口使用。
     * 较高范围[24:47]不能通过现场总线/PLC接口访问，因为它们是为外部RTDE客户端保留的。
     *
     * @par Python函数原型
     * getFloatInput(self: pyaubo_sdk.RegisterControl, arg0: int) -> float
     *
     * @par Lua函数原型
     * getFloatInput(address: number) -> number
     *
     */
    float getFloatInput(uint32_t address);

    /**
     *
     * @param address
     * @param value
     * @return
     *
     * @note 只有在实现 RTDE/Modbus Slave/PLC 服务端时使用
     *
     * @par Python函数原型
     * setFloatInput(self: pyaubo_sdk.RegisterControl, arg0: int, arg1: float)
     * -> int
     *
     * @par Lua函数原型
     * setFloatInput(address: number, value: number) -> nil
     *
     */
    int setFloatInput(uint32_t address, float value);

    /**
     *
     * @param address
     * @return
     *
     * @par Python函数原型
     * getDoubleInput(self: pyaubo_sdk.RegisterControl, arg0: int) -> float
     *
     * @par Lua函数原型
     * getDoubleInput(address: number) -> number
     *
     */
    double getDoubleInput(uint32_t address);

    /**
     *
     * @param address
     * @param value
     * @return
     *
     * @note 只有在实现 RTDE/Modbus Slave/PLC 服务端时使用
     *
     * @par Python函数原型
     * setDoubleInput(self: pyaubo_sdk.RegisterControl, arg0: int, arg1: float)
     * -> int
     *
     * @par Lua函数原型
     * setDoubleInput(address: number, value: number) -> nil
     *
     */
    int setDoubleInput(uint32_t address, double value);

    /**
     * Reads the boolean from one of the output registers, which can also be
     * accessed by a Field bus.
     * Note, uses it’s own memory space.
     *
     * 从一个输出寄存器中读取布尔值，也可以通过现场总线进行访问。
     * 注意，它使用自己的内存空间。
     *
     * @param address Address of the register (0:127)
     * 寄存器地址（0:127）
     * @return The boolean value held by the register (true, false)
     * 寄存器中保存的布尔值（true, false）
     *
     * @note The lower range of the boolean output registers [0:63] is reserved
     * for FieldBus/PLC interface usage. The upper range [64:127] cannot be
     * accessed by FieldBus/PLC interfaces, since it is reserved for external
     * RTDE clients
     * 布尔输出寄存器的较低范围[0:63]保留供现场总线/PLC接口使用。
     * 较高范围[64:127]不能通过现场总线/PLC接口访问，因为它们是为外部RTDE客户端保留的。
     *
     * @par Python函数原型
     * getBoolOutput(self: pyaubo_sdk.RegisterControl, arg0: int) -> bool
     *
     * @par Lua函数原型
     * getBoolOutput(address: number) -> boolean
     *
     */
    bool getBoolOutput(uint32_t address);

    /**
     *
     * @param address
     * @param value
     * @return
     *
     * @par Python函数原型
     * setBoolOutput(self: pyaubo_sdk.RegisterControl, arg0: int, arg1: bool) ->
     * int
     *
     * @par Lua函数原型
     * setBoolOutput(address: number, value: boolean) -> nil
     *
     */
    int setBoolOutput(uint32_t address, bool value);

    /**
     * Reads the integer from one of the output registers, which can also be
     * accessed by a Field bus. Note, uses it’s own memory space.
     *
     * 从一个输出寄存器中读取整数值，也可以通过现场总线进行访问。
     * 注意，它使用自己的内存空间。
     *
     * @param address Address of the register (0:47)
     * 寄存器地址（0:47）
     * @return The int value held by the register [-2,147,483,648 :
     * 2,147,483,647]
     * 寄存器中保存的整数值（-2,147,483,648 : 2,147,483,647）
     *
     * @note The lower range of the integer output registers [0:23] is reserved
     * for FieldBus/PLC interface usage. The upper range [24:47] cannot be
     * accessed by FieldBus/PLC interfaces, since it is reserved for external
     * RTDE clients.
     * 整数输出寄存器的较低范围[0:23]保留供现场总线/PLC接口使用。
     * 较高范围[24:47]不能通过现场总线/PLC接口访问，因为它们是为外部RTDE客户端保留的。
     *
     * @par Python函数原型
     * getInt32Output(self: pyaubo_sdk.RegisterControl, arg0: int) -> int
     *
     * @par Lua函数原型
     * getInt32Output(address: number) -> number
     *
     */
    int getInt32Output(uint32_t address);

    /**
     *
     * @param address
     * @param value
     * @return
     *
     * @par Python函数原型
     * setInt32Output(self: pyaubo_sdk.RegisterControl, arg0: int, arg1: int) ->
     * int
     *
     * @par Lua函数原型
     * setInt32Output(address: number, value: number) -> nil
     *
     */
    int setInt32Output(uint32_t address, int value);

    /**
     * Reads the float from one of the output registers, which can also be
     * accessed by a Field bus. Note, uses it’s own memory space.
     *
     * 从一个输出寄存器中读取浮点数，也可以通过现场总线进行访问。
     * 注意，它使用自己的内存空间。
     *
     * @param address Address of the register (0:47)
     * 寄存器地址（0:47）
     * @return The value held by the register (float)
     * 寄存器中保存的浮点数值（float）
     *
     * @note The lower range of the float output registers [0:23] is reserved
     * for FieldBus/PLC interface usage. The upper range [24:47] cannot be
     * accessed by FieldBus/PLC interfaces, since it is reserved for external
     * RTDE clients.
     * 浮点数输出寄存器的较低范围[0:23]保留供现场总线/PLC接口使用。
     * 较高范围[24:47]不能通过现场总线/PLC接口访问，因为它们是为外部RTDE客户端保留的。
     *
     * @par Python函数原型
     * getFloatOutput(self: pyaubo_sdk.RegisterControl, arg0: int) -> float
     *
     * @par Lua函数原型
     * getFloatOutput(address: number) -> number
     *
     */
    float getFloatOutput(uint32_t address);

    /**
     *
     * @param address
     * @param value
     * @return
     *
     * @par Python函数原型
     * setFloatOutput(self: pyaubo_sdk.RegisterControl, arg0: int, arg1: float)
     * -> int
     *
     * @par Lua函数原型
     * setFloatOutput(address: number, value: number) -> nil
     *
     */
    int setFloatOutput(uint32_t address, float value);

    /**
     *
     * @param address
     * @return
     *
     * @par Python函数原型
     * getDoubleOutput(self: pyaubo_sdk.RegisterControl, arg0: int) -> float
     *
     * @par Lua函数原型
     * getDoubleOutput(address: number) -> number
     *
     */
    double getDoubleOutput(uint32_t address);

    /**
     *
     * @param address
     * @param value
     * @return
     *
     * @par Python函数原型
     * setDoubleOutput(self: pyaubo_sdk.RegisterControl, arg0: int, arg1: float)
     * -> int
     *
     * @par Lua函数原型
     * setDoubleOutput(address: number, value: number) -> nil
     *
     */
    int setDoubleOutput(uint32_t address, double value);

    /**
     * 用于 Modbus Slave
     *
     * @param address
     * @return
     *
     * @par Python函数原型
     * getInt16Register(self: pyaubo_sdk.RegisterControl, arg0: int) -> int
     *
     * @par Lua函数原型
     * getInt16Register(address: number) -> number
     *
     */
    int16_t getInt16Register(uint32_t address);

    /**
     *
     * @param address
     * @param value
     * @return
     *
     * @par Python函数原型
     * setInt16Register(self: pyaubo_sdk.RegisterControl, arg0: int, arg1: int)
     * -> int
     *
     * @par Lua函数原型
     * setInt16Register(address: number, value: number) -> nil
     *
     */
    int setInt16Register(uint32_t address, int16_t value);

    /**
     * 具名变量是否存在
     *
     * @param key 变量名
     * @return
     */
    bool hasNamedVariable(const std::string &key);

    /**
     * 获取具名变量的类型
     *
     * @param key
     * @return
     */
    std::string getNamedVariableType(const std::string &key);

    /**
     * 具名变量是否更新
     *
     * @param key
     * @param since
     * @return
     *
     * @par Python函数原型
     * variableUpdated(self: pyaubo_sdk.RegisterControl, arg0: str, arg1: int)
     * -> bool
     *
     * @par Lua函数原型
     * variableUpdated(key: string, since: number) -> boolean
     *
     */
    bool variableUpdated(const std::string &key, uint64_t since);

    /**
     * 获取变量值
     *
     * @param key
     * @param default_value
     * @return
     *
     * @par Python函数原型
     * getBool(self: pyaubo_sdk.RegisterControl, arg0: str, arg1: bool) -> bool
     *
     * @par Lua函数原型
     * getBool(key: string, default_value: boolean) -> boolean
     *
     */
    bool getBool(const std::string &key, bool default_value);

    /**
     * 设置/更新变量值
     *
     * @param key
     * @param value
     * @return
     *
     * @par Python函数原型
     * setBool(self: pyaubo_sdk.RegisterControl, arg0: str, arg1: bool) -> int
     *
     * @par Lua函数原型
     * setBool(key: string, value: boolean) -> nil
     *
     */
    int setBool(const std::string &key, bool value);

    /**
     * 获取变量值
     *
     * @param key
     * @param default_value
     * @return
     *
     * @par Python函数原型
     * getVecChar(self: pyaubo_sdk.RegisterControl, arg0: str, arg1: List[str])
     * -> List[str]
     *
     * @par Lua函数原型
     * getVecChar(key: string, default_value: table) -> table
     *
     */
    std::vector<char> getVecChar(const std::string &key,
                                 const std::vector<char> &default_value);

    /**
     * 设置/更新变量值
     *
     * @param key
     * @param value
     * @return
     *
     * @par Python函数原型
     * setVecChar(self: pyaubo_sdk.RegisterControl, arg0: str, arg1: List[str])
     * -> int
     *
     * @par Lua函数原型
     * setVecChar(key: string, value: table) -> nil
     *
     */
    int setVecChar(const std::string &key, const std::vector<char> &value);

    /**
     * 获取变量值
     *
     * @param key
     * @param default_value
     * @return
     *
     * @par Python函数原型
     * getInt32(self: pyaubo_sdk.RegisterControl, arg0: str, arg1: int) -> int
     *
     * @par Lua函数原型
     * getInt32(key: string, default_value: number) -> number
     *
     */
    int getInt32(const std::string &key, int default_value);

    /**
     * 设置/更新变量值
     *
     * @param key
     * @param value
     * @return
     *
     * @par Python函数原型
     * setInt32(self: pyaubo_sdk.RegisterControl, arg0: str, arg1: int) -> int
     *
     * @par Lua函数原型
     * setInt32(key: string, value: number) -> nil
     *
     */
    int setInt32(const std::string &key, int value);

    /**
     * 获取变量值
     *
     * @param key
     * @param default_value
     * @return
     *
     * @par Python函数原型
     * getVecInt32(self: pyaubo_sdk.RegisterControl, arg0: str, arg1: List[int])
     * -> List[int]
     *
     * @par Lua函数原型
     * getVecInt32(key: string, default_value: table) -> table
     *
     */
    std::vector<int32_t> getVecInt32(const std::string &key,
                                     const std::vector<int32_t> &default_value);

    /**
     * 设置/更新变量值
     *
     * @param key
     * @param value
     * @return
     *
     * @par Python函数原型
     * setVecInt32(self: pyaubo_sdk.RegisterControl, arg0: str, arg1: List[int])
     * -> int
     *
     * @par Lua函数原型
     * setVecInt32(key: string, value: table) -> nil
     *
     */
    int setVecInt32(const std::string &key, const std::vector<int32_t> &value);

    /**
     * 获取变量值
     *
     * @param key
     * @param default_value
     * @return
     *
     * @par Python函数原型
     * getFloat(self: pyaubo_sdk.RegisterControl, arg0: str, arg1: float) ->
     * float
     *
     * @par Lua函数原型
     * getFloat(key: string, default_value: number) -> number
     *
     */
    float getFloat(const std::string &key, float default_value);

    /**
     * 设置/更新变量值
     *
     * @param key
     * @param value
     * @return
     *
     * @par Python函数原型
     * setFloat(self: pyaubo_sdk.RegisterControl, arg0: str, arg1: float) -> int
     *
     * @par Lua函数原型
     * setFloat(key: string, value: number) -> nil
     *
     */
    int setFloat(const std::string &key, float value);

    /**
     * 获取变量值
     *
     * @param key
     * @param default_value
     * @return
     *
     * @par Python函数原型
     * getVecFloat(self: pyaubo_sdk.RegisterControl, arg0: str, arg1:
     * List[float]) -> List[float]
     *
     * @par Lua函数原型
     * getVecFloat(key: string, default_value: table) -> table
     *
     */
    std::vector<float> getVecFloat(const std::string &key,
                                   const std::vector<float> &default_value);

    /**
     * 设置/更新变量值
     *
     * @param key
     * @param value
     * @return
     *
     * @par Python函数原型
     * setVecFloat(self: pyaubo_sdk.RegisterControl, arg0: str, arg1:
     * List[float]) -> int
     *
     * @par Lua函数原型
     * setVecFloat(key: string, value: table) -> nil
     *
     */
    int setVecFloat(const std::string &key, const std::vector<float> &value);

    /**
     * 获取变量值
     *
     * @param key
     * @param default_value
     * @return
     *
     * @par Python函数原型
     * getDouble(self: pyaubo_sdk.RegisterControl, arg0: str, arg1: float) ->
     * float
     *
     * @par Lua函数原型
     * getDouble(key: string, default_value: number) -> number
     *
     */
    double getDouble(const std::string &key, double default_value);

    /**
     * 设置/更新变量值
     *
     * @param key
     * @param value
     * @return
     *
     * @par Python函数原型
     * setDouble(self: pyaubo_sdk.RegisterControl, arg0: str, arg1: float) ->
     * int
     *
     * @par Lua函数原型
     * setDouble(key: string, value: number) -> nil
     *
     */
    int setDouble(const std::string &key, double value);

    /**
     * 获取变量值
     *
     * @param key
     * @param default_value
     * @return
     *
     * @par Python函数原型
     * getVecDouble(self: pyaubo_sdk.RegisterControl, arg0: str, arg1:
     * List[float]) -> List[float]
     *
     * @par Lua函数原型
     * getVecDouble(key: string, default_value: table) -> table
     *
     */
    std::vector<double> getVecDouble(const std::string &key,
                                     const std::vector<double> &default_value);

    /**
     * 设置/更新变量值
     *
     * @param key
     * @param value
     * @return
     *
     * @par Python函数原型
     * setVecDouble(self: pyaubo_sdk.RegisterControl, arg0: str, arg1:
     * List[float]) -> int
     *
     * @par Lua函数原型
     * setVecDouble(key: string, value: table) -> nil
     *
     */
    int setVecDouble(const std::string &key, const std::vector<double> &value);

    /**
     * 获取变量值
     *
     * @param key
     * @param default_value
     * @return
     *
     * @par Python函数原型
     * getString(self: pyaubo_sdk.RegisterControl, arg0: str, arg1: str) -> str
     *
     * @par Lua函数原型
     * getString(key: string, default_value: string) -> string
     *
     */
    std::string getString(const std::string &key,
                          const std::string &default_value);

    /**
     * 设置/更新变量值
     *
     * @param key
     * @param value
     * @return
     *
     * @par Python函数原型
     * setString(self: pyaubo_sdk.RegisterControl, arg0: str, arg1: str) -> int
     *
     * @par Lua函数原型
     * setString(key: string, value: string) -> nil
     *
     */
    int setString(const std::string &key, const std::string &value);

    /**
     * 清除变量
     *
     * @param key
     * @return
     *
     * @par Python函数原型
     * clearNamedVariable(self: pyaubo_sdk.RegisterControl, arg0: str) -> int
     *
     * @par Lua函数原型
     * clearNamedVariable(key: string) -> nil
     *
     */
    int clearNamedVariable(const std::string &key);

    /**
     * 设置看门狗
     *
     * 看门狗被触发之后控制器会执行对应的动作，并自动删除看门狗
     *
     * @param key
     * @param timeout 超时时间，单位秒(s)，超时时间最小为 0.1s
     * @param action
     *   NONE (0): 无动作
     *   PAUSE(1): 暂停运行时
     *   STOP (2): 停止运行时/停止机器人运动
     *   PROTECTIVE_STOP (3): 触发防护停止
     * @return
     */
    int setWatchDog(const std::string &key, double timeout, int action);

    /**
     * 获取看门狗动作
     *
     * @param key
     * @return
     */
    int getWatchDogAction(const std::string &key);

    /**
     * 获取看门狗超时时间
     *
     * @param key
     * @return
     */
    int getWatchDogTimeout(const std::string &key);

    /**
     * Adds a new modbus signal for the controller to supervise. Expects no
     * response.
     *
     * 添加一个新的Modbus信号以供控制器监视。不需要返回响应。
     *
     * @param device_info is rtu format.
     * eg,"serial_port,baud,parity,data_bit,stop_bit" \n (1)The serial_port
     * argument specifies the name of the serial port eg. On Linux ,"/dev/ttyS0"
     * or "/dev/ttyUSB0". On Windows,
     * \\.\COM10". \n
     * (2)The baud argument specifies the baud rate of the communication, eg.
     * 9600, 19200, 57600, 115200, etc. \n
     * (3)parity:N for none,E for even,O for odd. \n
     * (4)data_bit:The data_bits argument specifies the number of bits of data,
     * the allowed values are 5, 6, 7 and 8. \n
     * (5)stop_bit:The stop_bits argument
     * specifies the bits of stop, the allowed values are 1 and 2.
     *
     * device_info is tcp format.eg,"ip address,port" \n
     * (1)The ip address parameter specifies the ip address of the server \n
     * (2)The port parameter specifies the port number that the server is
     * listening on.
     *
     * 设备信息 \n
     * 设备信息是RTU格式，
     * 例如："serial_port,baud,parity,data_bit,stop_bit" \n
     * (1)serial_port参数指定串口的名称，
     * 例如，在Linux上为"/dev/ttyS0"或"/dev/ttyUSB0"，在Windows上为"\.\COM10"\n
     * (2)baud参数指定通信的波特率，例如9600、19200、57600、115200等 \n
     * (3)parity参数指定奇偶校验方式，N表示无校验，E表示偶校验，O表示奇校验 \n
     * (4)data_bit参数指定数据位数，允许的值为5、6、7和8 \n
     * (5)stop_bit参数指定停止位数，允许的值为1和2
     *
     * 设备信息是TCP格式，例如："ip address,port" \n
     * (1)ip address参数指定服务器的IP地址 \n
     * (2)port参数指定服务器监听的端口号
     * @param slave_number An integer normally not used and set to 255, but is
     * a free choice between 0 and 255.
     * 通常不使用，设置为255即可，但可以在0到255之间自由选择
     * @param signal_address An integer specifying the address of the either
     * the coil or the register that this new signal should reflect. Consult
     * the configuration of the modbus unit for this information.
     * 指定新信号应该反映的线圈或寄存器的地址。
     * 请参考Modbus单元的配置以获取此信息。
     * @param signal_type An integer specifying the type of signal to add. 0 =
     * digital input, 1 = digital output, 2 = register input and 3 = register
     * output.
     * 指定要添加的信号类型。
     * 0 = 数字输入，1 = 数字输出，2 = 寄存器输入，3 = 寄存器输出。
     * @param signal_name  A string uniquely identifying the signal. If a
     * string is supplied which is equal to an already added signal, the new
     * signal will replace the old one. The length of the string cannot exceed
     * 20 characters.
     * 唯一标识信号的名词。
     * 如果提供的字符串与已添加的信号相等，则新信号将替换旧信号。
     * 字符串的长度不能超过20个字符。
     * @param sequential_mode Setting to True forces the modbus client to wait
     * for a response before sending the next request. This mode is required by
     * some fieldbus units (Optional).
     * 设置为True会强制Modbus客户端在发送下一个请求之前等待响应。
     * 某些fieldbus单元需要此模式。
     * 可选参数。
     * @return
     *
     * @par Python函数原型
     * modbusAddSignal(self: pyaubo_sdk.RegisterControl, arg0: str, arg1: int,
     * arg2: int, arg3: int, arg4: str, arg5: bool) -> int
     *
     * @par Lua函数原型
     * modbusAddSignal(device_info: string, slave_number: number,
     * signal_address: number, signal_type: number, signal_name: string,
     * sequential_mode: boolean) -> nil
     *
     */
    int modbusAddSignal(const std::string &device_info, int slave_number,
                        int signal_address, int signal_type,
                        const std::string &signal_name, bool sequential_mode);

    /**
     * Deletes the signal identified by the supplied signal name.
     * 删除指定名称的信号。
     *
     * @param signal_name A string equal to the name of the signal that should
     * be deleted.
     * 要删除的信号的名称
     * @return
     *
     * @par Python函数原型
     * modbusDeleteSignal(self: pyaubo_sdk.RegisterControl, arg0: str) -> int
     *
     * @par Lua函数原型
     * modbusDeleteSignal(signal_name: string) -> nil
     *
     */
    int modbusDeleteSignal(const std::string &signal_name);

    /**
     * Delete all modbus signals
     *
     * @return
     */
    int modbusDeleteAllSignals();

    /**
     * Reads the current value of a specific signal.
     * 读取特定信号的当前值。
     *
     * @param signal_name A string equal to the name of the signal for which
     * the value should be gotten.
     * 要获取值的信号的名称
     * @return An integer or a boolean. For digital signals: 1 or 0. For
     * register signals: The register value expressed as an integer.If the
     * value is -1, it means the signal does not exist
     * 对于数字信号：1或0。
     * 对于寄存器信号：表示为整数的寄存器值。如果值为-1，则表示该信号不存在。
     *
     * @par Python函数原型
     * modbusGetSignalStatus(self: pyaubo_sdk.RegisterControl, arg0: str) -> int
     *
     * @par Lua函数原型
     * modbusGetSignalStatus(signal_name: string) -> nil
     *
     */
    int modbusGetSignalStatus(const std::string &signal_name);

    /**
     * 获取所有信号的名字集合
     *
     * @return 所有信号的名字集合
     */
    std::vector<std::string> modbusGetSignalNames();

    /**
     * 获取所有信号的类型集合
     *
     * @return 所有信号的类型集合
     */
    std::vector<int> modbusGetSignalTypes();

    /**
     * 获取所有信号的数值集合
     *
     * @return 所有信号的数值集合
     */
    std::vector<int> modbusGetSignalValues();

    /**
     * 获取所有信号的请求是否有错误(0:无错误,其他:有错误)集合
     *
     * @return ModbusErrorNum
     */
    std::vector<int> modbusGetSignalErrors();

    /**
     * Sends a command specified by the user to the modbus unit located on the
     * specified IP address. Cannot be used to request data, since the response
     * will not be received. The user is responsible for supplying data which
     * is meaningful to the supplied function code. The builtin function takes
     * care of constructing the modbus frame, so the user should not be
     * concerned with the length of the command.
     *
     * 将用户指定的命令发送到指定IP地址上的Modbus单元。
     * 由于不会接收到响应，因此不能用于请求数据。
     * 用户负责提供对所提供的功能码有意义的数据。
     * 内置函数负责构建Modbus帧，因此用户不需要关心命令的长度。
     *
     * @param device_info is rtu format.
     * eg,"serial_port,baud,parity,data_bit,stop_bit" \n
     * (1)The serial_port argument specifies the name of the serial port eg. On
     * Linux
     * ,"/dev/ttyS0" or "/dev/ttyUSB0". On Windows,
     * \\.\COM10". \n
     * (2)The baud argument specifies the baud rate of the communication, eg.
     * 9600, 19200, 57600, 115200, etc. \n
     * (3)parity:N for none,E for even,O for odd. \n
     * (4)data_bit:The data_bits argument specifies the number of bits of data,
     * the allowed values are 5, 6, 7 and 8. \n
     * (5)stop_bit:The stop_bits argument
     * specifies the bits of stop, the allowed values are 1 and 2.
     *
     * device_info is tcp format.eg,"ip address,port"
     *
     * 设备信息 \n
     * 设备信息是RTU格式，
     * 例如："serial_port,baud,parity,data_bit,stop_bit" \n
     * (1)serial_port参数指定串口的名称，
     * 例如，在Linux上为"/dev/ttyS0"或"/dev/ttyUSB0"，在Windows上为"\.\COM10"\n
     * (2)baud参数指定通信的波特率，例如9600、19200、57600、115200等 \n
     * (3)parity参数指定奇偶校验方式，N表示无校验，E表示偶校验，O表示奇校验 \n
     * (4)data_bit参数指定数据位数，允许的值为5、6、7和8 \n
     * (5)stop_bit参数指定停止位数，允许的值为1和2
     *
     * 设备信息是TCP格式，例如："ip address,port" \n
     * (1)ip address参数指定服务器的IP地址 \n
     * (2)port参数指定服务器监听的端口号
     * @param slave_number An integer specifying the slave number to use for
     * the custom command.
     * 指定用于自定义命令的从站号
     * @param function_code An integer specifying the function code for the
     * custom command.
     * 指定自定义命令的功能码
     *
     * Modbus function codes
     * MODBUS_FC_READ_COILS                0x01
     * MODBUS_FC_READ_DISCRETE_INPUTS      0x02
     * MODBUS_FC_READ_HOLDING_REGISTERS    0x03
     * MODBUS_FC_READ_INPUT_REGISTERS      0x04
     * MODBUS_FC_WRITE_SINGLE_COIL         0x05
     * MODBUS_FC_WRITE_SINGLE_REGISTER     0x06
     * MODBUS_FC_READ_EXCEPTION_STATUS     0x07
     * MODBUS_FC_WRITE_MULTIPLE_COILS      0x0F
     * MODBUS_FC_WRITE_MULTIPLE_REGISTERS  0x10
     * MODBUS_FC_REPORT_SLAVE_ID           0x11
     * MODBUS_FC_MASK_WRITE_REGISTER       0x16
     * MODBUS_FC_WRITE_AND_READ_REGISTERS  0x17
     *
     * @param data An array of integers in which each entry must be a valid
     * byte (0-255) value.
     * 必须是有效的字节值（0-255）
     * @return
     *
     * @par Python函数原型
     * modbusSendCustomCommand(self: pyaubo_sdk.RegisterControl, arg0: str,
     * arg1: int, arg2: int, arg3: List[int]) -> int
     *
     * @par Lua函数原型
     * modbusSendCustomCommand(device_info: string, slave_number: number,
     * function_code: number, data: table) -> nil
     *
     */
    int modbusSendCustomCommand(const std::string &device_info,
                                int slave_number, int function_code,
                                const std::vector<uint8_t> &data);

    /**
     * Sets the selected digital input signal to either a "default" or
     * "freedrive" action.
     * 将选择的数字输入信号设置为“default”或“freedrive”
     *
     * @param robot_name A string identifying a robot name that conncted robot
     * 连接的机器人名称
     * @param signal_name A string identifying a digital input signal that was
     * previously added.
     * 先前被添加的数字输入信号
     * @param action The type of action. The action can either be "default" or
     * "freedrive". (string)
     * 操作类型。操作可以是“default”或“freedrive”
     * @return
     *
     * @par Python函数原型
     * modbusSetDigitalInputAction(self: pyaubo_sdk.RegisterControl, arg0: str,
     * arg1: str, arg2: int)
     *
     * @par Lua函数原型
     * modbusSetDigitalInputAction(robot_name: string, signal_name: string,
     * action: number) -> nil
     *
     */
    int modbusSetDigitalInputAction(const std::string &robot_name,
                                    const std::string &signal_name,
                                    StandardInputAction action);

    /**
     * 设置 Modbus 信号输出动作
     *
     * @param robot_name
     * @param signal_name
     * @param runstate
     * @return
     */
    int modbusSetOutputRunstate(const std::string &robot_name,
                                const std::string &signal_name,
                                StandardOutputRunState runstate);

    /**
     * Sets the output register signal identified by the given name to the
     * given value.
     * 将指定名称的输出寄存器信号设置为给定的值
     *
     * @param signal_name A string identifying an output register signal that
     * in advance has been added.
     * 提前被添加的输出寄存器信号
     * @param value An integer which must be a valid word (0-65535)
     * 必须是有效的整数，范围是 0-65535
     * @return
     *
     * @par Python函数原型
     * modbusSetOutputSignal(self: pyaubo_sdk.RegisterControl, arg0: str, arg1:
     * int) -> int
     *
     * @par Lua函数原型
     * modbusSetOutputSignal(signal_name: string, value: number) -> nil
     *
     */
    int modbusSetOutputSignal(const std::string &signal_name, uint16_t value);

    /**
     * 设置modbus信号输出脉冲(仅支持线圈输出类型)
     *
     * @param signal_name: A string identifying an output register signal that
     * in advance has been added.
     * 提前被添加的输出寄存器信号
     * @param value: An integer which must be a valid word (0-65535)
     * 必须是有效的整数，范围是 0-65535
     * @param duration: Duration of the signal, in seconds
     * @return
     *
     * @par Python函数原型
     * modbusSetOutputSignalPulse(self: pyaubo_sdk.RegisterControl, arg0: str,
     * arg1: int, arg2 double) -> int
     *
     * @par Lua函数原型
     * modbusSetOutputSignalPulse(signal_name: string, value: number, duration:
     * number) -> nil
     *
     */
    int modbusSetOutputSignalPulse(const std::string &signal_name,
                                   uint16_t value, double duration);

    /**
     * Sets the frequency with which the robot will send requests to the Modbus
     * controller to either read or write the signal value.
     * 设置机器人向Modbus控制器发送请求的频率，用于读取或写入信号值
     *
     * @param signal_name A string identifying an output digital signal that
     * in advance has been added.
     * 提前被添加的输出数字信号
     * @param update_frequency An integer in the range 0-125 specifying the
     * update frequency in Hz.
     * 更新频率（以赫兹为单位），范围是0-125
     * @return
     *
     * @par Python函数原型
     * modbusSetSignalUpdateFrequency(self: pyaubo_sdk.RegisterControl, arg0:
     * str, arg1: int) -> int
     *
     * @par Lua函数原型
     * modbusSetSignalUpdateFrequency(signal_name: string, update_frequency:
     * number) -> nil
     *
     */
    int modbusSetSignalUpdateFrequency(const std::string &signal_name,
                                       int update_frequency);

    /**
     * 获取指定 modbus 信号索引，从0开始，不能存在则返回-1
     *
     * @param signal_name
     * @return
     */
    int modbusGetSignalIndex(const std::string &signal_name);

    /**
     * 获取指定 modbus 信号的错误状态
     *
     * @param signal_name
     * @return 返回错误代码 ModbusErrorNum
     */
    int modbusGetSignalError(const std::string &signal_name);

    /**
     * 获取指定 modbus 设备的连接状态
     *
     * @param device_name
     * 设备名是TCP格式，"ip:port", 例如："127.0.0.1:502" \n
     * 设备名是RTU格式，"serial_port", 例如："/dev/ttyUSB0" \n
     *
     * @return
     * 0: 表示设备处于连接状态
     * -1: 表示设备不存在
     * -2: 表示设备处于断开状态
     *
     */
    int getModbusDeviceStatus(const std::string &device_name);

protected:
    void *d_;
};
using RegisterControlPtr = std::shared_ptr<RegisterControl>;

// clang-format off
#define RegisterControl_DECLARES                                \
    _FUNC(RegisterControl, 1, getBoolInput, address)            \
    _INST(RegisterControl, 2, setBoolInput, address, value)     \
    _FUNC(RegisterControl, 1, getInt32Input, address)           \
    _INST(RegisterControl, 2, setInt32Input, address, value)    \
    _FUNC(RegisterControl, 1, getFloatInput, address)           \
    _INST(RegisterControl, 2, setFloatInput, address, value)    \
    _FUNC(RegisterControl, 1, getDoubleInput, address)          \
    _INST(RegisterControl, 2, setDoubleInput, address, value)   \
    _FUNC(RegisterControl, 1, getBoolOutput, address)           \
    _INST(RegisterControl, 2, setBoolOutput, address, value)    \
    _FUNC(RegisterControl, 1, getInt32Output, address)          \
    _INST(RegisterControl, 2, setInt32Output, address, value)   \
    _FUNC(RegisterControl, 1, getFloatOutput, address)          \
    _INST(RegisterControl, 2, setFloatOutput, address, value)   \
    _FUNC(RegisterControl, 1, getDoubleOutput, address)         \
    _INST(RegisterControl, 2, setDoubleOutput, address, value)  \
    _FUNC(RegisterControl, 1, getInt16Register, address)        \
    _INST(RegisterControl, 2, setInt16Register, address, value) \
    _FUNC(RegisterControl, 2, variableUpdated, key, since)      \
    _FUNC(RegisterControl, 1, hasNamedVariable, key)            \
    _FUNC(RegisterControl, 1, getNamedVariableType, key)        \
    _FUNC(RegisterControl, 2, getBool, key, default_value)      \
    _INST(RegisterControl, 2, setBool, key, value)              \
    _FUNC(RegisterControl, 2, getVecChar, key, default_value)   \
    _INST(RegisterControl, 2, setVecChar, key, value)           \
    _FUNC(RegisterControl, 2, getInt32, key, default_value)     \
    _INST(RegisterControl, 2, setInt32, key, value)             \
    _FUNC(RegisterControl, 2, getVecInt32, key, default_value)  \
    _INST(RegisterControl, 2, setVecInt32, key, value)          \
    _FUNC(RegisterControl, 2, getFloat, key, default_value)     \
    _INST(RegisterControl, 2, setFloat, key, value)             \
    _FUNC(RegisterControl, 2, getVecFloat, key, default_value)  \
    _INST(RegisterControl, 2, setVecFloat, key, value)          \
    _FUNC(RegisterControl, 2, getDouble, key, default_value)    \
    _INST(RegisterControl, 2, setDouble, key, value)            \
    _FUNC(RegisterControl, 2, getVecDouble, key, default_value) \
    _INST(RegisterControl, 2, setVecDouble, key, value)         \
    _FUNC(RegisterControl, 2, getString, key, default_value)    \
    _INST(RegisterControl, 2, setString, key, value)            \
    _INST(RegisterControl, 1, clearNamedVariable, key)          \
    _INST(RegisterControl, 3, setWatchDog, key, timeout, action) \
    _FUNC(RegisterControl, 1, getWatchDogAction, key)           \
    _FUNC(RegisterControl, 1, getWatchDogTimeout, key)           \
    _INST(RegisterControl, 6, modbusAddSignal, device_info, slave_number, signal_address, signal_type, signal_name, sequential_mode) \
    _INST(RegisterControl, 1, modbusDeleteSignal, signal_name) \
    _INST(RegisterControl, 0, modbusDeleteAllSignals)          \
    _FUNC(RegisterControl, 1, modbusGetSignalStatus, signal_name) \
    _FUNC(RegisterControl, 0, modbusGetSignalNames)            \
    _FUNC(RegisterControl, 0, modbusGetSignalTypes)            \
    _FUNC(RegisterControl, 0, modbusGetSignalValues)           \
    _FUNC(RegisterControl, 0, modbusGetSignalErrors)           \
    _INST(RegisterControl, 4, modbusSendCustomCommand, IP, slave_number, function_code, data) \
    _INST(RegisterControl, 3, modbusSetDigitalInputAction, robot_name, signal_name, action) \
    _INST(RegisterControl, 3, modbusSetOutputRunstate, robot_name, signal_name, runstate)   \
    _INST(RegisterControl, 2, modbusSetOutputSignal, signal_name, value) \
    _INST(RegisterControl, 3, modbusSetOutputSignalPulse, signal_name, value, duration) \
    _INST(RegisterControl, 2, modbusSetSignalUpdateFrequency, signal_name, update_frequency) \
    _FUNC(RegisterControl, 1, modbusGetSignalIndex, signal_name) \
    _FUNC(RegisterControl, 1, modbusGetSignalError, signal_name) \
    _FUNC(RegisterControl, 1, getModbusDeviceStatus, device_name)
// clang-format on
} // namespace common_interface
} // namespace arcs

#endif // AUBO_SDK_REGISTER_CONTROL_INTERFACE_H
