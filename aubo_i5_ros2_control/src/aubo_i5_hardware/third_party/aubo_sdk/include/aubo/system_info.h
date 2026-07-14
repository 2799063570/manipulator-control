/** @file  system_info.h
 *  @brief 获取系统信息接口，如接口板的版本号、示教器软件的版本号
 */
#ifndef AUBO_SDK_SYSTEM_INFO_INTERFACE_H
#define AUBO_SDK_SYSTEM_INFO_INTERFACE_H

#include <stdint.h>
#include <string>
#include <memory>

#include <aubo/global_config.h>

namespace arcs {
namespace common_interface {

class ARCS_ABI_EXPORT SystemInfo
{
public:
    SystemInfo();
    virtual ~SystemInfo();

    /**
     * 获取控制器软件版本号
     *
     * @return 返回控制器软件版本号
     *
     * @par Python函数原型
     * getControlSoftwareVersionCode(self: pyaubo_sdk.SystemInfo) -> int
     *
     * @par Lua函数原型
     * getControlSoftwareVersionCode() -> number
     *
     * @par C++示例
     * @code
     * int control_version =
     * rpc_cli->getSystemInfo()->getControlSoftwareVersionCode();
     * @endcode
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"SystemInfo.getControlSoftwareVersionCode","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":28003}
     *
     */
    int getControlSoftwareVersionCode();

    /**
     * 获取完整控制器软件版本号
     *
     * @return 返回完整控制器软件版本号
     *
     * @par Python函数原型
     * getControlSoftwareFullVersion(self: pyaubo_sdk.SystemInfo) -> str
     *
     * @par Lua函数原型
     * getControlSoftwareFullVersion() -> string
     *
     * @par C++示例
     * @code
     * std::string control_version =
     * rpc_cli->getSystemInfo()->getControlSoftwareFullVersion();
     * @endcode
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"SystemInfo.getControlSoftwareFullVersion","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":"0.31.0-alpha.16+20alc76"}
     *
     */
    std::string getControlSoftwareFullVersion();

    /**
     * 获取接口版本号
     *
     * @return 返回接口版本号
     *
     * @par Python函数原型
     * getInterfaceVersionCode(self: pyaubo_sdk.SystemInfo) -> int
     *
     * @par Lua函数原型
     * getInterfaceVersionCode() -> number
     *
     * @par C++示例
     * @code
     * int interface_version =
     * rpc_cli->getSystemInfo()->getInterfaceVersionCode();
     * @endcode
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"SystemInfo.getInterfaceVersionCode","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":22003}
     *
     */
    int getInterfaceVersionCode();

    /**
     * 获取控制器软件构建时间
     *
     * @return 返回控制器软件构建时间
     *
     * @par Python函数原型
     * getControlSoftwareBuildDate(self: pyaubo_sdk.SystemInfo) -> str
     *
     * @par Lua函数原型
     * getControlSoftwareBuildDate() -> string
     *
     * @par C++示例
     * @code
     * std::string build_date =
     * rpc_cli->getSystemInfo()->getControlSoftwareBuildDate();
     * @endcode
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"SystemInfo.getControlSoftwareBuildDate","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":"2024-3-5 07:03:20"}
     *
     */
    std::string getControlSoftwareBuildDate();

    /**
     * 获取控制器软件git版
     *
     * @return 返回控制器软件git版本
     *
     * @par Python函数原型
     * getControlSoftwareVersionHash(self: pyaubo_sdk.SystemInfo) -> str
     *
     * @par Lua函数原型
     * getControlSoftwareVersionHash() -> string
     *
     * @par C++示例
     * @code
     * std::string git_version =
     * rpc_cli->getSystemInfo()->getControlSoftwareVersionHash();
     * @endcode
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"SystemInfo.getControlSoftwareVersionHash","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":"fa4f64a"}
     *
     */
    std::string getControlSoftwareVersionHash();

    /**
     * 获取系统时间(软件启动时间 ns 纳秒)
     *
     * @return 返回系统时间(软件启动时间 ns 纳秒)
     *
     * @par Python函数原型
     * getControlSystemTime(self: pyaubo_sdk.SystemInfo) -> int
     *
     * @par Lua函数原型
     * getControlSystemTime() -> number
     *
     * @par C++示例
     * @code
     * std::string system_time =
     * rpc_cli->getSystemInfo()->getControlSystemTime();
     * @endcode
     *
     * @par JSON-RPC请求示例
     * {"jsonrpc":"2.0","method":"SystemInfo.getControlSystemTime","params":[],"id":1}
     *
     * @par JSON-RPC响应示例
     * {"id":1,"jsonrpc":"2.0","result":9287799079682}
     *
     */
    uint64_t getControlSystemTime();

protected:
    void *d_;
};

using SystemInfoPtr = std::shared_ptr<SystemInfo>;

// clang-format off
#define SystemInfo_DECLARES                              \
    _FUNC(SystemInfo, 0, getControlSoftwareVersionCode)  \
    _FUNC(SystemInfo, 0, getControlSoftwareFullVersion)  \
    _FUNC(SystemInfo, 0, getInterfaceVersionCode)        \
    _FUNC(SystemInfo, 0, getControlSoftwareBuildDate)    \
    _FUNC(SystemInfo, 0, getControlSoftwareVersionHash)  \
    _FUNC(SystemInfo, 0, getControlSystemTime)
// clang-format on
} // namespace common_interface
} // namespace arcs
#endif
