#include "aubo_sdk/rpc.h"
#ifdef WIN32
#include <windows.h>
#endif

#include "unistd.h"
#include <cstring>
#include <fstream>

using namespace arcs::common_interface;
using namespace arcs::aubo_sdk;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const std::vector<T> &vd)
{
    if (vd.size() == 0) {
        os << "<none>";
        return os;
    }
    for (size_t i = 0; i < vd.size(); i++) {
        os << vd[i];
        if (i < vd.size() - 1) {
            os << ", ";
        }
    }
    return os;
}

// 实现阻塞功能: 当机械臂运动到目标路点时，程序再往下执行
int waitArrival(const std::vector<AxisInterfacePtr> &axis,
                const std::vector<std::string> names,
                const std::vector<double> way)
{
    int cnt_steady = 0;
    int cnt = 0;

    while (true) {
        for (size_t i = 0; i < names.size(); i++) {
            std::cout << "real pos: " << axis[i]->getExtAxisPosition()
                      << " cmd pos: " << way[i] << std::endl;
            if (std::abs(axis[i]->getExtAxisPosition() - way[i]) < 10e-5) {
                cnt_steady++;
            }
        }
        if (cnt_steady == (int)names.size()) {
            return 0;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        if (cnt++ > 1000) {
            cnt = 0;
            return -1;
        }
    }

    return 0;
}

/**
 * 功能: 机械臂关节运动
 * 步骤:
 * 第一步: 设置 RPC 超时、连接 RPC 服务、机械臂登录
 * 第二步: 设置运动速度比率、以关节运动的方式依次经过3个路点
 * 第三步: RPC 退出登录、断开连接
 */

void exampleMovej(RpcClientPtr cli)
{
    std::vector<double> servo_r_j(6, 0.0);
    std::vector<double> servo_ex_j(3, 0.0);
    std::vector<double> way1(3, 0.0);

    std::vector<AxisInterfacePtr> axis;
    auto names = cli->getAxisNames();

    axis.resize(names.size());
    for (size_t i = 0; i < names.size(); i++) {
        axis[i] = cli->getAxisInterface(names[i]);
    }

    printf("name: %s\n", names[0].c_str());
    axis[1]->followAnotherAxis(names[0], 0.0, 0.0);
    std::cout << "axis: " << axis.size() << std::endl;
    std::cout << "names: " << names.size() << std::endl;

    for (;;) {
        for (size_t i = 0; i < names.size(); i++) {
            way1[i] = -3.0;
            axis[i]->moveExtJoint(way1[i], 3.0, 1.0, 0.5);
        }

        int ret = waitArrival(axis, names, way1);
        if (ret == 0) {
            std::cout << "关节运动到路点1成功" << std::endl;
        } else {
            std::cout << "关节运动到路点1失败" << std::endl;
        }

        for (size_t i = 0; i < names.size(); i++) {
            way1[i] = 3.0;
            axis[i]->moveExtJoint(way1[i], 3.0, 1.0, 0.5);
        }

        ret = waitArrival(axis, names, way1);
        if (ret == 0) {
            std::cout << "关节运动到路点2成功" << std::endl;
        } else {
            std::cout << "关节运动到路点2失败" << std::endl;
        }
    }
}

#define LOCAL_IP "172.19.19.112"

int main(int argc, char **argv)
{
#ifdef WIN32
    // 将Windows控制台输出代码页设置为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
#endif

    auto rpc_cli = std::make_shared<RpcClient>();
    // 接口调用: 设置 RPC 超时
    rpc_cli->setRequestTimeout(1000);
    // 接口调用: 连接到 RPC 服务

    rpc_cli->connect("172.19.19.112", 30004);
    // 接口调用: 登录
    rpc_cli->login("aubo", "123456");

    // 关节运动
    exampleMovej(rpc_cli);

    // 接口调用: 退出登录
    rpc_cli->logout();
    // 接口调用: 断开连接
    rpc_cli->disconnect();

    return 0;
}
