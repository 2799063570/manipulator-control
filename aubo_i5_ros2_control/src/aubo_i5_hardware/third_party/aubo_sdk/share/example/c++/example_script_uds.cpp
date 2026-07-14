#include <thread>
#include <fstream>
#include "aubo_sdk/rpc.h"
#include "aubo_sdk/script.h"
#ifdef WIN32
#include <Windows.h>
#endif
using namespace arcs::common_interface;
using namespace arcs::aubo_sdk;
using namespace std;

/**
 * 功能: 运行脚本程序
 * 步骤:
 * 第一步: 连接 SCRIPT 服务、机械臂登录
 * 第二步: 输入脚本文件名
 * 第三步: 读取文件，运行脚本程序。如果打开文件失败，则退出程序。
 */
// 本示例程序需要发送一个可运行的脚本到arcs控制器,example/c++中test_io.lua文件是提供的一个脚本，运行程序前先将test_io.lua拷贝到可执行程序所在路径，然后运行example_script可执行程序，输入lua脚本名字(test_io.lua)
// 运行前请注意机械臂周围无障碍物并且安全,遇到紧急情况按下急停按钮

#define LOCAL_IP "127.0.0.1"

int main(int argc, char **argv)
{
#ifdef WIN32
    // 将Windows控制台输出代码页设置为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
#endif

    auto script = std::make_shared<ScriptClient>(1);
    // 接口调用: 连接到 SCRIPT 服务
    script->connect();
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

    // 读取并打印脚本文件中的内容
    std::string str_all;
    while (!file.eof()) {
        std::string str_line;
        getline(file, str_line);
        str_all += str_line;
        str_all += "\n";
    }
    str_all += "\r\n\r\n";
    cout << "脚本内容:" << endl << str_all << endl;
    file.close();

    // 发送脚本内容到控制器
    script->sendString(str_all);

    // 增加阻塞来保证执行完脚本
    while (1) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
