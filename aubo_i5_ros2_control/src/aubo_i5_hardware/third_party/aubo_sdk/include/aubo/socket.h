/** @file  socket.h
 *  @brief socket通信
 */
#ifndef AUBO_SDK_SOCKET_INTERFACE_H
#define AUBO_SDK_SOCKET_INTERFACE_H

#include <vector>
#include <memory>

#include <aubo/type_def.h>
#include <aubo/global_config.h>

namespace arcs {
namespace common_interface {

class ARCS_ABI_EXPORT Socket
{
public:
    Socket();
    virtual ~Socket();

    /**
     * Open TCP/IP ethernet communication socket
     *
     * Instruction
     *
     * @param address
     * @param port
     * @param socket_name
     * @return
     *
     * @par Python函数原型
     * socketOpen(self: pyaubo_sdk.Socket, arg0: str, arg1: int, arg2: str) ->
     * int
     *
     * @par Lua函数原型
     * socketOpen(address: string, port: number, socket_name: string) -> nil
     *
     */
    int socketOpen(const std::string &address, int port,
                   const std::string &socket_name = "socket_0");

    /**
     * Closes TCP/IP socket communication
     * Closes down the socket connection to the server.
     *
     * Instruction
     *
     * @param socket_name
     * @return
     *
     * @par Python函数原型
     * socketClose(self: pyaubo_sdk.Socket, arg0: str) -> int
     *
     * @par Lua函数原型
     * socketClose(socket_name: string) -> nil
     *
     */
    int socketClose(const std::string &socket_name = "socket_0");

    /**
     * Reads a number of ascii formatted floats from the socket. A maximum
     * of 30 values can be read in one command.
     * A list of numbers read (list of floats, length=number+1)
     *
     * Instruction
     *
     * Result will be store in a register named reg_key. Use getFloatVec
     * to retrive data
     *
     * @param number
     * @param variable
     * @param socket_name
     * @return
     *
     * @par Python函数原型
     * socketReadAsciiFloat(self: pyaubo_sdk.Socket, arg0: int, arg1: str, arg2:
     * str) -> int
     *
     * @par Lua函数原型
     * socketReadAsciiFloat(number: number, variable: string, socket_name:
     * string) -> number
     *
     */
    int socketReadAsciiFloat(int number, const std::string &variable,
                             const std::string &socket_name = "socket_0");

    /**
     * Reads a number of 32 bit integers from the socket. Bytes are in
     * network byte order. A maximum of 30 values can be read in one
     * command.
     * A list of numbers read (list of ints, length=number+1)
     *
     * Instruction
     *
     * std::vector<int>
     *
     * @param number
     * @param variable
     * @param socket_name
     * @return
     *
     * @par Python函数原型
     * socketReadBinaryInteger(self: pyaubo_sdk.Socket, arg0: int, arg1: str,
     * arg2: str) -> int
     *
     * @par Lua函数原型
     * socketReadBinaryInteger(number: number, variable: string, socket_name:
     * string) -> number
     *
     */
    int socketReadBinaryInteger(int number, const std::string &variable,
                                const std::string &socket_name = "socket_0");

    /**
     * Reads a number of bytes from the socket. Bytes are in network byte
     * order. A maximum of 30 values can be read in one command.
     * A list of numbers read (list of ints, length=number+1)
     *
     * Instruction
     *
     * std::vector<char>
     *
     * @param number
     * @param variable
     * @param socket_name
     * @return
     *
     * @par Python函数原型
     * socketReadByteList(self: pyaubo_sdk.Socket, arg0: int, arg1: str, arg2:
     * str) -> int
     *
     * @par Lua函数原型
     * socketReadByteList(number: number, variable: string, socket_name: string)
     * -> number
     *
     */
    int socketReadByteList(int number, const std::string &variable,
                           const std::string &socket_name = "socket_0");

    /**
     * Reads all data from the socket and returns the data as a string.
     * Bytes are in network byte order.
     *
     * The optional parameters "prefix" and "suffix", can be used to express
     * what is extracted from the socket. The "prefix" specifies the start
     * of the substring (message) extracted from the socket. The data up to
     * the end of the "prefix" will be ignored and removed from the socket.
     * The "suffix" specifies the end of the substring (message) extracted
     * from the socket. Any remaining data on the socket, after the "suffix",
     * will be preserved. E.g. if the socket server sends a string
     * "noise>hello<", the controller can receive the "hello" by calling this
     * script function with the prefix=">" and suffix="<". By using the
     * "prefix" and "suffix" it is also possible send multiple string to the
     * controller at once, because the suffix defines where the message ends.
     * E.g. sending ">hello<>world<"
     *
     * Instruction
     *
     * std::string
     *
     * @param variable
     * @param socket_name
     * @param prefix
     * @param suffix
     * @param interpret_escape
     * @return
     *
     * @par Python函数原型
     * socketReadString(self: pyaubo_sdk.Socket, arg0: str, arg1: str, arg2:
     * str, arg3: str, arg4: bool) -> int
     *
     * @par Lua函数原型
     * socketReadString(variable: string, socket_name: string, prefix: string,
     * suffix: string, interpret_escape: boolean) -> number
     *
     */
    int socketReadString(const std::string &variable,
                         const std::string &socket_name = "socket_0",
                         const std::string &prefix = "",
                         const std::string &suffix = "",
                         bool interpret_escape = false);

    /**
     * Instruction
     * std::vector<char>
     *
     * @param variable
     * @param socket_name
     * @return
     *
     * @par Python函数原型
     * socketReadAllString(self: pyaubo_sdk.Socket, arg0: str, arg1: str) -> int
     *
     * @par Lua函数原型
     * socketReadAllString(variable: string, socket_name: string) -> number
     *
     */
    int socketReadAllString(const std::string &variable,
                            const std::string &socket_name = "socket_0");

    /**
     * Sends a byte to the server
     * Sends the byte <value> through the socket. Expects no response. Can
     * be used to send special ASCII characters; 10 is newline, 2 is start of
     * text, 3 is end of text.
     *
     * Instruction
     *
     * @param value
     * @param socket_name
     * @return
     *
     * @par Python函数原型
     * socketSendByte(self: pyaubo_sdk.Socket, arg0: str, arg1: str) -> int
     *
     * @par Lua函数原型
     * socketSendByte(value: string, socket_name: string) -> nil
     *
     */
    int socketSendByte(char value, const std::string &socket_name = "socket_0");

    /**
     * Sends an int (int32_t) to the server
     * Sends the int <value> through the socket. Send in network byte order.
     * Expects no response
     *
     * Instruction
     *
     * @param value
     * @param socket_name
     * @return
     *
     * @par Python函数原型
     * socketSendInt(self: pyaubo_sdk.Socket, arg0: int, arg1: str) -> int
     *
     * @par Lua函数原型
     * socketSendInt(value: number, socket_name: string) -> nil
     *
     */
    int socketSendInt(int value, const std::string &socket_name = "socket_0");

    /**
     * Sends a string with a newline character to the server - useful for
     * communicatin with the UR dashboard server
     * Sends the string <str> through the socket in ASCII coding. Expects no
     * response.
     *
     * Instruction
     *
     * @param str
     * @param socket_name
     * @return
     *
     * @par Python函数原型
     * socketSendLine(self: pyaubo_sdk.Socket, arg0: str, arg1: str) -> int
     *
     * @par Lua函数原型
     * socketSendLine(str: string, socket_name: string) -> nil
     *
     */
    int socketSendLine(const std::string &str,
                       const std::string &socket_name = "socket_0");

    /**
     * Sends a string to the server
     * Sends the string <str> through the socket in ASCII coding. Expects no
     * response.
     *
     * Instruction
     *
     * @param str
     * @param socket_name
     * @return
     *
     * @par Python函数原型
     * socketSendString(self: pyaubo_sdk.Socket, arg0: str, arg1: str) -> int
     *
     * @par Lua函数原型
     * socketSendString(str: string, socket_name: string) -> nil
     *
     */
    int socketSendString(const std::string &str,
                         const std::string &socket_name = "socket_0");

    /**
     *
     * @param is_check
     * @param str
     * @param socket_name
     * @return
     *
     * @par Python函数原型
     * socketSendAllString(self: pyaubo_sdk.Socket, arg0: bool, arg1: List[str],
     * arg2: str) -> int
     *
     * @par Lua函数原型
     * socketSendAllString(is_check: boolean, str: table, socket_name: string)
     * -> nil
     *
     */
    int socketSendAllString(bool is_check, const std::vector<char> &str,
                            const std::string &socket_name = "socket_0");

protected:
    void *d_;
};
using SocketPtr = std::shared_ptr<Socket>;

// clang-format off
#define Socket_DECLARES                                                                \
    _INST(Socket, 3, socketOpen, address, port, socket_name)                               \
    _INST(Socket, 1, socketClose, socket_name)                                             \
    _FUNC(Socket, 3, socketReadAsciiFloat, number, variable, socket_name)                       \
    _FUNC(Socket, 3, socketReadBinaryInteger, number, variable, socket_name)                    \
    _FUNC(Socket, 3, socketReadByteList, number, variable, socket_name)                         \
    _FUNC(Socket, 5, socketReadString, variable, socket_name, prefix, suffix, interpret_escape) \
    _FUNC(Socket, 2, socketReadAllString, variable, socket_name)                                \
    _INST(Socket, 2, socketSendByte, value, socket_name)                                   \
    _INST(Socket, 2, socketSendInt, value, socket_name)                                    \
    _INST(Socket, 2, socketSendLine, str, socket_name)                                     \
    _INST(Socket, 2, socketSendString, str, socket_name)                                   \
    _INST(Socket, 3, socketSendAllString, is_check, str, socket_name)
// clang-format on

} // namespace common_interface
} // namespace arcs
#endif
