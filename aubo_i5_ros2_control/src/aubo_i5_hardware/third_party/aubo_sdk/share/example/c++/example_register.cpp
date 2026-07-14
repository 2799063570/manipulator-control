#include "aubo_sdk/rpc.h"
#include "thread"

#define LOCAL_IP           "127.0.0.1"
#define MODBUS_IP          "172.16.3.111,502"
#define MODBUS_SERIAL_PORT "/dev/ttyUSB0,9600,N,8,1"

using namespace arcs::common_interface;
using namespace arcs::aubo_sdk;

void printfTitle(std::string str)
{
    std::cout << std::endl << std::endl << str << std::endl;
}

void exampleRegisterInput(RpcClientPtr impl)
{
    printfTitle("->Register_Bool_Input_0_63");
    for (int i = 0; i < 63; i++) {
        if (0 == i % 2) {
            impl->getRegisterControl()->setBoolInput(i, true);
        } else {
            impl->getRegisterControl()->setBoolInput(i, false);
        }
        std::cout << (int)impl->getRegisterControl()->getBoolInput(i) << ",";
    }

    printfTitle("->Register_Bool_Input_64_127");
    for (int i = 64; i < 128; i++) {
        if (0 == i % 2) {
            impl->getRegisterControl()->setBoolInput(i, true);
        } else {
            impl->getRegisterControl()->setBoolInput(i, false);
        }
        std::cout << (int)impl->getRegisterControl()->getBoolInput(i) << ",";
    }

    printfTitle("->Register_Int_Input");
    for (int i = 0; i < 64; i++) {
        if (0 == i % 2) {
            impl->getRegisterControl()->setInt32Input(i, i);
        } else {
            impl->getRegisterControl()->setInt32Input(i, i);
        }
        std::cout << impl->getRegisterControl()->getInt32Input(i) << ",";
    }

    printfTitle("->Register_Float_Input");
    for (int i = 0; i < 64; i++) {
        if (0 == i % 2) {
            impl->getRegisterControl()->setFloatInput(i, 0.1);
        } else {
            impl->getRegisterControl()->setFloatInput(i, 0.2);
        }
        std::cout << impl->getRegisterControl()->getFloatInput(i) << ",";
    }

    printfTitle("->Register_Double_Input");
    for (int i = 0; i < 64; i++) {
        if (0 == i % 2) {
            impl->getRegisterControl()->setDoubleInput(i, 0.3);
        } else {
            impl->getRegisterControl()->setDoubleInput(i, 0.4);
        }
        std::cout << impl->getRegisterControl()->getDoubleInput(i) << ",";
    }
}

void exampleRegisterOutput(RpcClientPtr impl)
{
    printfTitle("->Register_Bool_Output_0_63");
    for (int i = 0; i < 63; i++) {
        if (0 == i % 2) {
            impl->getRegisterControl()->setBoolOutput(i, true);
        } else {
            impl->getRegisterControl()->setBoolOutput(i, false);
        }
        std::cout << (int)impl->getRegisterControl()->getBoolOutput(i) << ",";
    }

    printfTitle("->Register_Bool_Output_64_127");
    for (int i = 64; i < 127; i++) {
        if (0 == i % 2) {
            impl->getRegisterControl()->setBoolOutput(i, true);
        } else {
            impl->getRegisterControl()->setBoolOutput(i, false);
        }
        std::cout << (int)impl->getRegisterControl()->getBoolOutput(i) << ",";
    }

    printfTitle("->Register_Int_Output");
    for (int i = 0; i < 64; i++) {
        if (0 == i % 2) {
            impl->getRegisterControl()->setInt32Output(i, i);
        } else {
            impl->getRegisterControl()->setInt32Output(i, i);
        }
        std::cout << impl->getRegisterControl()->getInt32Output(i) << ",";
    }

    printfTitle("->Register_Float_Output");
    for (int i = 0; i < 64; i++) {
        if (0 == i % 2) {
            impl->getRegisterControl()->setFloatOutput(i, 0.1);
        } else {
            impl->getRegisterControl()->setFloatOutput(i, 0.2);
        }
        std::cout << impl->getRegisterControl()->getFloatOutput(i) << ",";
    }

    printfTitle("->Register_Double_Output");
    for (int i = 0; i < 64; i++) {
        if (0 == i % 2) {
            impl->getRegisterControl()->setDoubleOutput(i, 0.3);
        } else {
            impl->getRegisterControl()->setDoubleOutput(i, 0.4);
        }
        std::cout << impl->getRegisterControl()->getDoubleOutput(i) << ",";
    }
}

void exampleRegisterCustom(RpcClientPtr impl)
{
    impl->getRegisterControl()->setBool("custom1", true);
    bool v_b = false;
    v_b = impl->getRegisterControl()->getBool("custom1", v_b);
    std::cout << std::endl
              << std::endl
              << "->Register_Bool_Custom:" << v_b << std::endl;

    impl->getRegisterControl()->setInt32("custom2", 34);
    int v_i = 0;
    v_i = impl->getRegisterControl()->getInt32("custom2", v_i);
    std::cout << std::endl
              << std::endl
              << "->Register_Int32_Custom:" << v_i << std::endl;

    impl->getRegisterControl()->setFloat("custom3", 0.1);
    float v_f = 0;
    v_f = impl->getRegisterControl()->getFloat("custom3", v_f);
    std::cout << std::endl
              << std::endl
              << "->Register_Float_Custom:" << v_f << std::endl;

    impl->getRegisterControl()->setDouble("custom4", 0.2);
    double v_d = 0;
    v_d = impl->getRegisterControl()->getDouble("custom4", v_d);
    std::cout << std::endl
              << std::endl
              << "->Register_Double_Custom:" << v_d << std::endl;

    impl->getRegisterControl()->setString("custom5", "test");
    std::string v_s = "";
    v_s = impl->getRegisterControl()->getString("custom5", v_s);
    std::cout << std::endl
              << std::endl
              << "->Register_String_Custom:" << v_s << std::endl;

    std::cout << std::endl
              << std::endl
              << "->Register_Clear_Custom:"
              << impl->getRegisterControl()->clearNamedVariable("custom5")
              << std::endl;
}

void exampleModbusTcp(RpcClientPtr impl)
{
    /**
        0 = read digital input
        1 = read digital output
        2 = read register input
        3 = read register output
        4 = write digital output
        5 = write register output
    **/

    // 发送自定义数据
    impl->getRegisterControl()->modbusSendCustomCommand(
        MODBUS_IP, 1, 0x06, { 0x00, 0x02, 0x00, 0x0F });

    // 写单个线圈
    impl->getRegisterControl()->modbusAddSignal(MODBUS_IP, 1, 0x00, 0x04,
                                                "WRITE_COLI_OUTPUT_01", true);
    auto res = impl->getRegisterControl()->modbusSetOutputSignal(
        "WRITE_COLI_OUTPUT_01", 0x01);

    // 写单个保持寄存器
    impl->getRegisterControl()->modbusAddSignal(
        MODBUS_IP, 1, 0x00, 0x05, "WRITE_HOLDING_REGISTER_OUTPUT_01", true);
    res = impl->getRegisterControl()->modbusSetOutputSignal(
        "WRITE_HOLDING_REGISTER_OUTPUT_01", 0x4455);

    // 读线圈寄存器（ip,站号,寄存器起始地址,功能码,名字）
    impl->getRegisterControl()->modbusAddSignal(MODBUS_IP, 1, 0x00, 0x01,
                                                "READ_COIL_00", true);
    impl->getRegisterControl()->modbusSetSignalUpdateFrequency("READ_COIL_00",
                                                               1);

    // 读离散输入寄存器
    impl->getRegisterControl()->modbusAddSignal(MODBUS_IP, 1, 0x00, 0x00,
                                                "READ_INPUT_00", true);
    impl->getRegisterControl()->modbusSetSignalUpdateFrequency("READ_INPUT_00",
                                                               1);

    // 读保持寄存器
    impl->getRegisterControl()->modbusAddSignal(
        MODBUS_IP, 1, 0x00, 0x03, "READ_HOLDING_REGISTER_00", true);
    impl->getRegisterControl()->modbusSetSignalUpdateFrequency(
        "READ_HOLDING_REGISTER_00", 1);

    // 读输入寄存器
    impl->getRegisterControl()->modbusAddSignal(MODBUS_IP, 1, 0x00, 0x02,
                                                "READ_INPUT_REGISTER_00", true);
    impl->getRegisterControl()->modbusSetSignalUpdateFrequency(
        "READ_INPUT_REGISTER_00", 1);

    // 等待对应信号的数据第一次获取完成
    std::this_thread::sleep_for(std::chrono::seconds(3));

    for (int i = 0; i < 100; i++) {
        std::cout << "-------------------------" << std::endl;
        auto ret =
            impl->getRegisterControl()->modbusGetSignalStatus("READ_COIL_00");
        std::cout << "READ_COIL_00:" << ret << std::endl;
        auto ret2 =
            impl->getRegisterControl()->modbusGetSignalStatus("READ_INPUT_00");
        std::cout << "READ_INPUT_00:" << ret2 << std::endl;
        auto ret3 = impl->getRegisterControl()->modbusGetSignalStatus(
            "READ_HOLDING_REGISTER_00");
        std::cout << "READ_HOLDING_REGISTER_00:" << ret3 << std::endl;
        auto ret4 = impl->getRegisterControl()->modbusGetSignalStatus(
            "READ_INPUT_REGISTER_00");
        std::cout << "READ_INPUT_REGISTER_00:" << ret4 << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    for (int i = 0; i < 10; i++) {
        impl->getRegisterControl()->modbusSetOutputSignal(
            "WRITE_COLI_OUTPUT_01", 0x01);
        impl->getRegisterControl()->modbusSetOutputSignal(
            "WRITE_COLI_OUTPUT_01", 0x00);
    }

    // 发送自定义数据
    impl->getRegisterControl()->modbusSendCustomCommand(
        MODBUS_IP, 1, 0x06, { 0x00, 0x03, 0x01, 0xFF });

    impl->getRegisterControl()->modbusDeleteSignal("WRITE_COLI_OUTPUT_01");
    impl->getRegisterControl()->modbusDeleteSignal(
        "WRITE_HOLDING_REGISTER_OUTPUT_01");
    impl->getRegisterControl()->modbusDeleteSignal("READ_COIL_00");
    impl->getRegisterControl()->modbusDeleteSignal("READ_INPUT_00");
    impl->getRegisterControl()->modbusDeleteSignal("READ_HOLDING_REGISTER_00");
    impl->getRegisterControl()->modbusDeleteSignal("READ_INPUT_REGISTER_00");
}

void exampleModbusRtu(RpcClientPtr impl)
{
    /**
        0 = read digital input
        1 = read digital output
        2 = read register input
        3 = read register output
        4 = write digital output
        5 = write register output
    **/
    // 发送自定义数据
    impl->getRegisterControl()->modbusSendCustomCommand(
        MODBUS_SERIAL_PORT, 1, 0x06, { 0x00, 0x02, 0x00, 0x0F, 0x68, 0x0E });

    // 写单个线圈
    impl->getRegisterControl()->modbusAddSignal(
        MODBUS_SERIAL_PORT, 1, 0x01, 0x04, "WRITE_COLI_OUTPUT_02", true);

    impl->getRegisterControl()->modbusSetOutputSignal("WRITE_COLI_OUTPUT_02",
                                                      0x01);

    // 写单个保持寄存器
    impl->getRegisterControl()->modbusAddSignal(
        MODBUS_SERIAL_PORT, 1, 0x00, 0x05, "WRITE_HOLDING_REGISTER_OUTPUT_01",
        true);

    impl->getRegisterControl()->modbusSetOutputSignal(
        "WRITE_HOLDING_REGISTER_OUTPUT_01", 0x4455);

    // 读线圈寄存器（ip,站号,寄存器起始地址,功能码,名字）
    impl->getRegisterControl()->modbusAddSignal(MODBUS_SERIAL_PORT, 1, 0x00,
                                                0x01, "READ_COIL_00", true);
    impl->getRegisterControl()->modbusSetSignalUpdateFrequency("READ_COIL_00",
                                                               1);

    // 读离散输入寄存器
    impl->getRegisterControl()->modbusAddSignal(MODBUS_SERIAL_PORT, 1, 0x00,
                                                0x00, "READ_INPUT_00", true);
    impl->getRegisterControl()->modbusSetSignalUpdateFrequency("READ_INPUT_00",
                                                               1);

    // 读保持寄存器
    impl->getRegisterControl()->modbusAddSignal(
        MODBUS_SERIAL_PORT, 1, 0x00, 0x03, "READ_HOLDING_REGISTER_00", true);
    impl->getRegisterControl()->modbusSetSignalUpdateFrequency(
        "READ_HOLDING_REGISTER_00", 1);

    // 读输入寄存器
    impl->getRegisterControl()->modbusAddSignal(
        MODBUS_SERIAL_PORT, 1, 0x00, 0x02, "READ_INPUT_REGISTER_00", true);
    impl->getRegisterControl()->modbusSetSignalUpdateFrequency(
        "READ_INPUT_REGISTER_00", 1);

    // 等待对应信号的数据第一次获取完成
    std::this_thread::sleep_for(std::chrono::seconds(2));

    for (int i = 0; i < 100; i++) {
        std::cout << "-------------------------" << i << std::endl;
        auto ret =
            impl->getRegisterControl()->modbusGetSignalStatus("READ_COIL_00");
        std::cout << "READ_COIL_00:" << ret << std::endl;

        auto ret2 =
            impl->getRegisterControl()->modbusGetSignalStatus("READ_INPUT_00");
        std::cout << "READ_INPUT_00:" << ret2 << std::endl;

        auto ret3 = impl->getRegisterControl()->modbusGetSignalStatus(
            "READ_HOLDING_REGISTER_00");
        std::cout << "READ_HOLDING_REGISTER_00:" << ret3 << std::endl;

        auto ret4 = impl->getRegisterControl()->modbusGetSignalStatus(
            "READ_INPUT_REGISTER_00");
        std::cout << "READ_INPUT_REGISTER_00:" << ret4 << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    for (int i = 0; i < 10; i++) {
        impl->getRegisterControl()->modbusSetOutputSignal(
            "WRITE_COLI_OUTPUT_02", 0x01);
        impl->getRegisterControl()->modbusSetOutputSignal(
            "WRITE_COLI_OUTPUT_02", 0x00);
    }

    impl->getRegisterControl()->modbusDeleteSignal("WRITE_COLI_OUTPUT_02");
    impl->getRegisterControl()->modbusDeleteSignal(
        "WRITE_HOLDING_REGISTER_OUTPUT_01");
    impl->getRegisterControl()->modbusDeleteSignal("READ_COIL_00");
    impl->getRegisterControl()->modbusDeleteSignal("READ_INPUT_00");
    impl->getRegisterControl()->modbusDeleteSignal("READ_HOLDING_REGISTER_00");
    impl->getRegisterControl()->modbusDeleteSignal("READ_INPUT_REGISTER_00");
}

void exampleModbusTcpAction(RpcClientPtr impl)
{
    /**
        0 = read digital input
        1 = read digital output
        2 = read register input
        3 = read register output
        4 = write digital output
        5 = write register output
    **/
    // 写单个线圈
    impl->getRegisterControl()->modbusAddSignal(MODBUS_IP, 1, 0x00, 0x04,
                                                "WRITE_COLI_OUTPUT_00", true);

    // 读线圈寄存器（ip,站号,寄存器起始地址,功能码,名字）
    impl->getRegisterControl()->modbusAddSignal(MODBUS_IP, 1, 0x00, 0x01,
                                                "READ_COIL_00", true);
    impl->getRegisterControl()->modbusSetSignalUpdateFrequency("READ_COIL_00",
                                                               1);

    // Default, 0,  "无触发"
    // Freedrive, 1, "触发拖动示教"
    // 设置动作为freedirver
    impl->getRegisterControl()->modbusSetDigitalInputAction(
        "rob1", "READ_COIL_00", StandardInputAction::Handguide);

    // 设置为1,打开freedrive
    auto res = impl->getRegisterControl()->modbusSetOutputSignal(
        "WRITE_COLI_OUTPUT_00", 0x01);

    std::this_thread::sleep_for(std::chrono::seconds(10));

    // 设置为0,关闭freedrive
    res = impl->getRegisterControl()->modbusSetOutputSignal(
        "WRITE_COLI_OUTPUT_00", 0x00);

    std::this_thread::sleep_for(std::chrono::seconds(10));
    impl->getRegisterControl()->modbusDeleteSignal("READ_COIL_00");
    impl->getRegisterControl()->modbusDeleteSignal("WRITE_COLI_OUTPUT_00");
}

int main(int argc, char **argv)
{
    // 测试slave:modsim32软件
    // arcs控制器中modbus-tcp更新周期是10ms
    // arcs控制器中modbus-rtu更新周期是40ms(为了保证指令响应成功,需要指令发送间隔)*寄存器类型数量+150ms(modbus_rtu请求用时)*寄存器类型数量+10ms(线程循环周期)。
    // modbus-rtu  波特率9600  同频率寄存器类型个数  读支持频率    写响应时间
    //                          1                   5hz      250ms左右
    //                          2                   2.5hz    500ms左右
    //                          3                   1.2hz    500ms左右
    //                          4                   1hz      500ms左右

    auto rpc_cli = std::make_shared<RpcClient>();
    rpc_cli->connect(LOCAL_IP, 30004);
    rpc_cli->login("aubo", "123456");
    rpc_cli->setRequestTimeout(5000); //有时候rtu连接需要两秒钟

    exampleRegisterInput(rpc_cli);
    exampleRegisterOutput(rpc_cli);
    exampleRegisterCustom(rpc_cli);
    exampleModbusTcp(rpc_cli);
    exampleModbusRtu(rpc_cli);
    // exampleModbusTcpAction(rpc_cli);
}
