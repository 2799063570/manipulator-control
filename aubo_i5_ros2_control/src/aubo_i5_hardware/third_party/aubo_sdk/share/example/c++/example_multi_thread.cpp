#include "aubo_sdk/rpc.h"
#ifdef WIN32
#include <Windows.h>
#endif
#define LOCAL_IP "127.0.0.1"

void read_test() {
  // RPC 客户端对象
  auto rpc_client = std::make_shared<arcs::aubo_sdk::RpcClient>();
  // 接口调用: 设置 RPC 超时
  rpc_client->setRequestTimeout(1000);
  // 接口调用: 连接到 RPC 服务
  rpc_client->connect(LOCAL_IP, 30004);
  // 接口调用: 登录
  rpc_client->login("aubo", "123456");
  // 接口调用: 获取机器人的名字
  auto robot_name = rpc_client->getRobotNames().front();
  auto impl = rpc_client->getRobotInterface(robot_name);

  // 创建一个包含10个线程的线程容器
  std::vector<std::thread> ths;
  for (int i = 0; i < 10; i++) {
    // 向线程容器中添加一个线程，每个线程都执行以下操作
    ths.emplace_back([&]() {
      while (1) {
        try {
          // 接口调用: 获取TCP偏移量
          impl->getRobotConfig()->getTcpOffset();
          // 输出一个'·'字符并刷新输出流
          std::cerr << "·" << std::flush;
        } catch (const arcs::aubo_sdk::AuboException &e) {
          // 打印异常信息
          std::cerr << std::this_thread::get_id() << ":" << e.what()
                    << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
      }
    });
  }

  // 等待所有线程完成
  for (auto &t : ths) {
    t.join();
  }
}

void connect_test() {
  // 创建一个包含10个线程的线程容器
  std::vector<std::thread> ths;
  for (int i = 0; i < 10; i++) {
    // 向线程容器中添加一个线程，每个线程都执行以下操作
    ths.emplace_back([&]() {
      // RPC 客户端对象
      auto rpc_client = std::make_shared<arcs::aubo_sdk::RpcClient>();
      // 接口调用: 设置 RPC 超时
      rpc_client->setRequestTimeout(1000);
      while (1) {
        try {
          // 接口调用: 连接到 RPC 服务
          rpc_client->connect(LOCAL_IP, 30004);
          // 接口调用: 登录
          rpc_client->login("aubo", "123456");
          std::this_thread::sleep_for(std::chrono::milliseconds(7));
          // 接口调用: 退出登录
          rpc_client->logout();
          // 接口调用: 断开 RPC 服务
          rpc_client->disconnect();
          std::this_thread::sleep_for(std::chrono::milliseconds(7));
        } catch (const arcs::aubo_sdk::AuboException &e) {
          // 打印异常信息
          std::cerr << std::this_thread::get_id() << ":" << e.what()
                    << std::endl;
        }
      }
    });
  }

  // 等待所有线程完成
  for (auto &t : ths) {
    t.join();
  }
}

int main(int argc, char **argv) {
#ifdef WIN32
  // 将Windows控制台输出代码页设置为 UTF-8
  SetConsoleOutputCP(CP_UTF8);
#endif
  connect_test();
  //  read_test();

  return 0;
}
