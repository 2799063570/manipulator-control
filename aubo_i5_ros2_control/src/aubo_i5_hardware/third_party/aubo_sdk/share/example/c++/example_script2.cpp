#include "aubo_sdk/rpc.h"
#include "aubo_sdk/script.h"
#include <fstream>
#include <thread>
#ifdef WIN32
#include <Windows.h>
#endif
using namespace arcs::common_interface;
using namespace arcs::aubo_sdk;
using namespace std;

/**
 * 功能: 运行本地脚本程序
 * 步骤:
 * 第一步: 连接 RPC 服务、机械臂登录
 * 第二步: 连接 SCRIPT 服务、机械臂登录
 * 第三步: 输入脚本文件名或者脚本绝对路径
 * 情况1：当lua脚本被拷贝到可执行程序所在路径（build/bin）下时，则输入脚本文件名
 * 情况2：当lua脚本没有在可执行程序所在路径（build/bin）下时，则输入脚本的绝对路径
 * 第四步: 读取文件，运行脚本程序。如果打开文件失败，则退出程序。
 */

#define LOCAL_IP "127.0.0.1"

int main(int argc, char **argv) {
#ifdef WIN32
  // 将Windows控制台输出代码页设置为 UTF-8
  SetConsoleOutputCP(CP_UTF8);
#endif
  auto rpc = std::make_shared<RpcClient>();
  // 接口调用: 设置 RPC 超时
  rpc->setRequestTimeout(1000);
  // 接口调用: 连接到 RPC 服务
  rpc->connect(LOCAL_IP, 30004);
  // 接口调用: 登录
  rpc->login("aubo", "123456");

  auto script = std::make_shared<ScriptClient>();
  // 接口调用: 连接到 SCRIPT 服务
  script->connect(LOCAL_IP, 30002);
  // 接口调用: 登录
  script->login("aubo", "123456");

  // 输入脚本文件名
  char file_name[20];
  cin >> file_name;

  // 打开文件
  ifstream file;
  file.open(file_name);
  // 如果打开文件失败，则退出程序
  if (!file) {
    cout << "open fail." << endl;
    exit(1);
  }

  file.close();

  // 接口调用: 发送本地的脚本文件到控制器
  script->sendFile(file_name);

  // 等待规划器启动
  int i = 0;
  while (1) {
    if (i++ > 5) {
      std::cerr << "规划器启动失败" << std::endl;
      return -1;
    }
    if (rpc->getRuntimeMachine()->getStatus() == RuntimeState::Running) {
      std::cout << "规划器启动成功，开始执行脚本" << std::endl;
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }

  // 等待规划器停止
  while (1) {
    if (rpc->getRuntimeMachine()->getStatus() == RuntimeState::Stopped) {
      std::cout << "规划器停止，脚本执行完成" << std::endl;
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }

  // 接口调用: 退出登录
  rpc->logout();
  // 接口调用: 断开连接
  rpc->disconnect();

  // 接口调用: 退出登录
  script->logout();
  // 接口调用: 断开连接
  script->disconnect();

  return 0;
}
