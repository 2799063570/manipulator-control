#include "aubo_sdk/rpc.h"
#include <math.h>
#include <thread>

using namespace arcs::common_interface;
using namespace arcs::aubo_sdk;

#define LOCAL_IP "127.0.0.1"

void example1(RpcClientPtr rpc_cli) {
  // 接口调用: 计时器 timer1 开始计时
  rpc_cli->getRuntimeMachine()->timerStart("timer1");
  std::this_thread::sleep_for(std::chrono::milliseconds(7500));
  // 接口调用: 计时器 timer1 结束计时
  rpc_cli->getRuntimeMachine()->timerStop("timer1");
  // 接口调用: 获取计时器 timer1 计时的时长
  std::cout << rpc_cli->getRuntimeMachine()->getTimer("timer1") << std::endl;

  // 接口调用: 计时器 timer1 开始计时
  rpc_cli->getRuntimeMachine()->timerStart("timer1");
  std::this_thread::sleep_for(std::chrono::milliseconds(7500));
  // 接口调用: 计时器 timer1 结束计时
  rpc_cli->getRuntimeMachine()->timerStop("timer1");
  // 接口调用: 获取计时器 timer1 计时的时长
  std::cout << rpc_cli->getRuntimeMachine()->getTimer("timer1") << std::endl;

  // 接口调用: 重置计时器 timer1
  rpc_cli->getRuntimeMachine()->timerReset("timer1");
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  // 接口调用: 获取计时器 timer1 计时的时长
  std::cout << rpc_cli->getRuntimeMachine()->getTimer("timer1") << std::endl;
  // 接口调用: 删除计时器 timer1
  rpc_cli->getRuntimeMachine()->timerDelete("timer1");
}

void example2(RpcClientPtr rpc_cli) {
  // 接口调用: 计时器 timer1 开始计时
  rpc_cli->getRuntimeMachine()->timerStart("timer1");
  std::this_thread::sleep_for(std::chrono::milliseconds(7500));
  // 接口调用: 获取计时器 timer1 计时的时长
  std::cout << rpc_cli->getRuntimeMachine()->getTimer("timer1") << std::endl;
  // 接口调用: 重置计时器 timer1
  rpc_cli->getRuntimeMachine()->timerReset("timer1");

  // 接口调用: 计时器 timer1 开始计时
  rpc_cli->getRuntimeMachine()->timerStart("timer1");
  std::this_thread::sleep_for(std::chrono::milliseconds(7500));
  // 接口调用: 获取计时器 timer1 计时的时长
  std::cout << rpc_cli->getRuntimeMachine()->getTimer("timer1") << std::endl;
  // 接口调用: 重置计时器 timer1
  rpc_cli->getRuntimeMachine()->timerReset("timer1");
  // 接口调用: 删除计时器 timer1
  rpc_cli->getRuntimeMachine()->timerDelete("timer1");
}

int main(int argc, char **argv) {
  auto rpc_cli = std::make_shared<RpcClient>();
  // 接口调用: 设置 RPC 超时
  rpc_cli->setRequestTimeout(1000);
  // 接口调用: 连接到 RPC 服务
  rpc_cli->connect(LOCAL_IP, 30004);
  // 接口调用: 登录
  rpc_cli->login("aubo", "123456");

  example1(rpc_cli);
  example2(rpc_cli);

  // 接口调用: 退出登录
  rpc_cli->logout();
  // 接口调用: 断开连接
  rpc_cli->disconnect();

  return 0;
}
