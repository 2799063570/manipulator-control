#include "aubo_sdk/rpc.h"

using namespace arcs::common_interface;
using namespace arcs::aubo_sdk;

#define LOCAL_IP "127.0.0.1"

void example_read_log(RpcClientPtr cli) {
  // 接口调用: 重新日志系统的格式
  cli->setLogHandler([](int level, const char *filename, int line,
                        const std::string &message) {
    std::cout << "level: " << level << std::endl;
    std::cout << "filename: " << filename << std::endl;
    std::cout << "line: " << line << std::endl;
    std::cout << "message: " << message << std::endl;
  });
}
/**
 * 功能: 自定义日志的格式
 * 步骤:
 * 第一步: 自定义日志的格式。
 * 注意: 要在连接服务器之前，调用 setLogHandler
 * 函数。否则，自定义的日志格式不会生效。
 * 第二步: 连接 RPC 服务、机械臂登录
 * 第三步: 退出登录、断开 RPC 服务
 */
int main() {
  // RPC 客户端对象
  auto rpc_cli = std::make_shared<RpcClient>();
  // 自定义日志的格式
  example_read_log(rpc_cli);
  // 接口调用: 设置 RPC 超时
  rpc_cli->setRequestTimeout(1000);
  // 接口调用: 连接到 RPC 服务
  rpc_cli->connect(LOCAL_IP, 30004);
  // 接口调用: 登录
  rpc_cli->login("aubo", "123456");

  // 接口调用: 退出登录
  rpc_cli->logout();
  // 接口调用: 断开连接
  rpc_cli->disconnect();
  return 0;
}
