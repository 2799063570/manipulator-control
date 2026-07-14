/** @file  serial.h
 *  @brief 串口通信
 */
#ifndef AUBO_SDK_SERIAL_INTERFACE_H
#define AUBO_SDK_SERIAL_INTERFACE_H

#include <vector>
#include <memory>

#include <aubo/type_def.h>
#include <aubo/global_config.h>

namespace arcs {
namespace common_interface {

class ARCS_ABI_EXPORT Serial
{
public:
    Serial();
    virtual ~Serial();

    /**
     * Open TCP/IP ethernet communication serial
     *
     * @param device
     * @param baud
     * @param stop_bits
     * @param even
     * @param serial_name
     * @return
     *
     * @par Python函数原型
     * serialOpen(self: pyaubo_sdk.Serial, arg0: str, arg1: int, arg2: float,
     * arg3: int, arg4: str) -> int
     *
     * @par Lua函数原型
     * serialOpen(device: string, baud: number, stop_bits: number, even: number,
     * serial_name: string) -> nil
     *
     */
    int serialOpen(const std::string &device, int baud, float stop_bits,
                   int even, const std::string &serial_name = "serial_0");

    /**
     * Closes TCP/IP serial communication
     * Closes down the serial connection to the server.
     *
     * @param serial_name
     * @return
     *
     * @par Python函数原型
     * serialClose(self: pyaubo_sdk.Serial, arg0: str) -> int
     *
     * @par Lua函数原型
     * serialClose(serial_name: string) -> nil
     *
     */
    int serialClose(const std::string &serial_name = "serial_0");

    /**
     * Reads a number of bytes from the serial. Bytes are in network byte
     * order. A maximum of 30 values can be read in one command.
     * A list of numbers read (list of ints, length=number+1)
     *
     * @param variable
     * @param serial_name
     * @return
     *
     * @par Python函数原型
     * serialReadByte(self: pyaubo_sdk.Serial, arg0: str, arg1: str) -> int
     *
     * @par Lua函数原型
     * serialReadByte(variable: string, serial_name: string) -> number
     *
     */
    int serialReadByte(const std::string &variable,
                       const std::string &serial_name = "serial_0");

    /**
     * Reads a number of bytes from the serial. Bytes are in network byte
     * order. A maximum of 30 values can be read in one command.
     * A list of numbers read (list of ints, length=number+1)
     *
     * @param number
     * @param variable
     * @param serial_name
     * @return
     *
     * @par Python函数原型
     * serialReadByteList(self: pyaubo_sdk.Serial, arg0: int, arg1: str, arg2:
     * str) -> int
     *
     * @par Lua函数原型
     * serialReadByteList(number: number, variable: string, serial_name: string)
     * -> number
     *
     */
    int serialReadByteList(int number, const std::string &variable,
                           const std::string &serial_name = "serial_0");

    /**
     * Reads all data from the serial and returns the data as a string.
     * Bytes are in network byte order.
     *
     * The optional parameters "prefix" and "suffix", can be used to express
     * what is extracted from the serial. The "prefix" specifies the start
     * of the substring (message) extracted from the serial. The data up to
     * the end of the "prefix" will be ignored and removed from the serial.
     * The "suffix" specifies the end of the substring (message) extracted
     * from the serial. Any remaining data on the serial, after the "suffix",
     * will be preserved. E.g. if the serial server sends a string
     * "noise>hello<", the controller can receive the "hello" by calling this
     * script function with the prefix=">" and suffix="<". By using the
     * "prefix" and "suffix" it is also possible send multiple string to the
     * controller at once, because the suffix defines where the message ends.
     * E.g. sending ">hello<>world<"
     *
     * @param variable
     * @param serial_name
     * @param prefix
     * @param suffix
     * @param interpret_escape
     * @return
     *
     * @par Python函数原型
     * serialReadString(self: pyaubo_sdk.Serial, arg0: str, arg1: str, arg2:
     * str, arg3: str, arg4: bool) -> int
     *
     * @par Lua函数原型
     * serialReadString(variable: string, serial_name: string, prefix: string,
     * suffix: string, interpret_escape: boolean) -> number
     *
     */
    int serialReadString(const std::string &variable,
                         const std::string &serial_name = "serial_0",
                         const std::string &prefix = "",
                         const std::string &suffix = "",
                         bool interpret_escape = false);

    /**
     * Sends a byte to the server
     * Sends the byte <value> through the serial. Expects no response. Can
     * be used to send special ASCII characters; 10 is newline, 2 is start of
     * text, 3 is end of text.
     *
     * @param value
     * @param serial_name
     * @return
     *
     * @par Python函数原型
     * serialSendByte(self: pyaubo_sdk.Serial, arg0: str, arg1: str) -> int
     *
     * @par Lua函数原型
     * serialSendByte(value: string, serial_name: string) -> nil
     *
     */
    int serialSendByte(char value, const std::string &serial_name = "serial_0");

    /**
     * Sends an int (int32_t) to the server
     * Sends the int <value> through the serial. Send in network byte order.
     * Expects no response
     *
     * @param value
     * @param serial_name
     * @return
     *
     * @par Python函数原型
     * serialSendInt(self: pyaubo_sdk.Serial, arg0: int, arg1: str) -> int
     *
     * @par Lua函数原型
     * serialSendInt(value: number, serial_name: string) -> nil
     *
     */
    int serialSendInt(int value, const std::string &serial_name = "serial_0");

    /**
     * Sends a string with a newline character to the server - useful for
     * communicatin with the UR dashboard server
     * Sends the string <str> through the serial in ASCII coding. Expects no
     * response.
     *
     * @param str
     * @param serial_name
     * @return
     *
     * @par Python函数原型
     * serialSendLine(self: pyaubo_sdk.Serial, arg0: str, arg1: str) -> int
     *
     * @par Lua函数原型
     * serialSendLine(str: string, serial_name: string) -> nil
     *
     */
    int serialSendLine(const std::string &str,
                       const std::string &serial_name = "serial_0");

    /**
     * Sends a string to the server
     * Sends the string <str> through the serial in ASCII coding. Expects no
     * response.
     *
     * @param str
     * @param serial_name
     * @return
     *
     * @par Python函数原型
     * serialSendString(self: pyaubo_sdk.Serial, arg0: str, arg1: str) -> int
     *
     * @par Lua函数原型
     * serialSendString(str: string, serial_name: string) -> nil
     *
     */
    int serialSendString(const std::string &str,
                         const std::string &serial_name = "serial_0");

    /**
     *
     * @param is_check
     * @param str
     * @param serial_name
     * @return
     *
     * @par Python函数原型
     * serialSendAllString(self: pyaubo_sdk.Serial, arg0: bool, arg1: List[str],
     * arg2: str) -> int
     *
     * @par Lua函数原型
     * serialSendAllString(is_check: boolean, str: table, serial_name: string)
     * -> nil
     *
     */
    int serialSendAllString(bool is_check, const std::vector<char> &str,
                            const std::string &serial_name = "serial_0");

protected:
    void *d_;
};
using SerialPtr = std::shared_ptr<Serial>;

// clang-format off
#define Serial_DECLARES                                                                    \
    _INST(Serial, 5, serialOpen, device, baud, stop_bits, even, serial_name)               \
    _INST(Serial, 1, serialClose, serial_name)                                             \
    _FUNC(Serial, 2, serialReadByte, variable, serial_name)                                     \
    _FUNC(Serial, 3, serialReadByteList, number, variable, serial_name)                         \
    _FUNC(Serial, 5, serialReadString, variable, serial_name, prefix, suffix, interpret_escape) \
    _INST(Serial, 2, serialSendByte, value, serial_name)                                   \
    _INST(Serial, 2, serialSendInt, value, serial_name)                                    \
    _INST(Serial, 2, serialSendLine, str, serial_name)                                     \
    _INST(Serial, 2, serialSendString, str, serial_name)                                   \
    _INST(Serial, 3, serialSendAllString, is_check, str, serial_name)
// clang-format on

} // namespace common_interface
} // namespace arcs
#endif
