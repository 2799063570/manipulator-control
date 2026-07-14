#include "example_9.h"

#include <thread>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <fstream>

#include <vector>

#include "AuboRobotMetaType.h" //Robot arm metadata type
#include "serviceinterface.h"  //Robot arm interface

//#define SERVER_HOST "192.168.1.100"
#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 30000

void Example_9::demo()
{
    ServiceInterface robotService;

    int ret = aubo_robot_namespace::InterfaceCallSuccCode;

    /** Interface call: login ***/
    ret = robotService.robotServiceLogin(SERVER_HOST, SERVER_PORT, "aubo",
                                         "123456");
    if (ret == aubo_robot_namespace::InterfaceCallSuccCode) {
        std::cout << "login successful." << std::endl;
    } else {
        std::cerr << "login failed." << std::endl;
    }

    // Get digital IO status
    double value;
    robotService.robotServiceGetBoardIOStatus(
        aubo_robot_namespace::RobotBoardUserDI, "U_DI_01", value);
    std::cout << "U_DI_01 status:" << value << std::endl;

    std::vector<aubo_robot_namespace::RobotIoType> ioType;
    std::vector<aubo_robot_namespace::RobotIoDesc> statusVector;

    ioType.push_back(aubo_robot_namespace::RobotBoardUserAI);
    robotService.robotServiceGetBoardIOStatus(ioType, statusVector);

    for (int i = 0; i < statusVector.size(); i++) {
        std::cout << statusVector[i].ioName << " " << statusVector[i].ioValue
                  << std::endl;
    }

    // Get the analog IO status
    robotService.robotServiceGetBoardIOStatus(
        aubo_robot_namespace::RobotBoardUserAI, "VI0", value);
    std::cerr << "---------VI0 status:" << value << std::endl;

    // Set the analog IO status
    ret = robotService.robotServiceSetBoardIOStatus(
        aubo_robot_namespace::RobotBoardUserDO, "U_DO_02", 0);
    ret = robotService.robotServiceSetBoardIOStatus(
        aubo_robot_namespace::RobotBoardUserAO, "VO1", 2);
    std::cerr << "ret:" << ret << std::endl;

    /** Interface call: logout　**/
    robotService.robotServiceLogout();
}

void Example_9::BoardiO_demo()
{
    ServiceInterface robotService;

    int ret = aubo_robot_namespace::InterfaceCallSuccCode;

    /** Interface call: login ***/
    ret = robotService.robotServiceLogin(SERVER_HOST, SERVER_PORT, "aubo",
                                         "123456");
    if (ret == aubo_robot_namespace::InterfaceCallSuccCode) {
        std::cout << "login successful." << std::endl;
    } else {
        std::cerr << "login failed." << std::endl;
    }
    /** Interface call: robotServiceGetBoardIOStatus ***/
    std::vector<aubo_robot_namespace::RobotIoType> ioType;
    ioType.push_back(aubo_robot_namespace::RobotBoardControllerDI);
    ioType.push_back(aubo_robot_namespace::RobotBoardControllerDO);
    ioType.push_back(aubo_robot_namespace::RobotBoardUserDI);
    ioType.push_back(aubo_robot_namespace::RobotBoardUserDO);
    ioType.push_back(aubo_robot_namespace::RobotBoardUserAI);
    ioType.push_back(aubo_robot_namespace::RobotBoardUserAO);

    std::vector<aubo_robot_namespace::RobotIoDesc> configVector;
    robotService.robotServiceGetBoardIOStatus(ioType, configVector);

    for (auto i = 0; i < int(configVector.size()); i++) {
        std::cout << "ioType is " << configVector[i].ioType << ",ioName is "
                  << configVector[i].ioName << ", ioValue is"
                  << configVector[i].ioValue << ", ioAddr is"
                  << configVector[i].ioAddr << std::endl;
    }
    /** Interface call: robotServiceSetBoardIOStatus ***/
    robotService.robotServiceSetBoardIOStatus(
        aubo_robot_namespace::RobotBoardUserAO, "CO2", 1);
    robotService.robotServiceSetBoardIOStatus(
        aubo_robot_namespace::RobotBoardUserDO, "U_DO_10", 1);
    /** Interface call: robotServiceIsOnlineMode ***/
    bool isOnlineMode, isOnlineMasterMode;
    robotService.robotServiceIsOnlineMode(isOnlineMode);
    robotService.robotServiceIsOnlineMasterMode(isOnlineMasterMode);
    std::cerr << "isOnlineMode is " << isOnlineMode << std::endl;
    std::cerr << "isOnlineMasterMode is " << isOnlineMasterMode << std::endl;
}

void Example_9::TooliO_demo()
{
    ServiceInterface robotService;

    int ret = aubo_robot_namespace::InterfaceCallSuccCode;

    /** Interface call: login ***/
    ret = robotService.robotServiceLogin(SERVER_HOST, SERVER_PORT, "aubo",
                                         "123456");
    if (ret == aubo_robot_namespace::InterfaceCallSuccCode) {
        std::cout << "login successful." << std::endl;
    } else {
        std::cerr << "login failed." << std::endl;
    }
    /** Interface call: robotServiceSetToolDOStatus ***/
    robotService.robotServiceSetToolDOStatus(
        aubo_robot_namespace::TOOL_DIGITAL_IO_3,
        aubo_robot_namespace::IO_STATUS_INVALID);
    robotService.robotServiceSetToolDOStatus(
        "TOOL_DIGITAL_IO_1", aubo_robot_namespace::IO_STATUS_VALID);

    /** Interface call: robotServiceGetAllToolAIStatus ***/
    std::vector<aubo_robot_namespace::RobotIoDesc> Vector;
    robotService.robotServiceGetAllToolAIStatus(Vector);
    robotService.robotServiceGetAllToolDigitalIOStatus(Vector);
    for (auto i = 0; i < int(Vector.size()); i++) {
        std::cout << "ioName is " << Vector[i].ioName << "ioType is "
                  << Vector[i].ioType << "ioValue is" << Vector[i].ioValue
                  << std::endl;
    }
}
void Example_9::SafetyConfig_demo()
{
    ServiceInterface robotService;

    int ret = aubo_robot_namespace::InterfaceCallSuccCode;

    /** Interface call: login ***/
    ret = robotService.robotServiceLogin(SERVER_HOST, SERVER_PORT, "aubo",
                                         "123456");
    if (ret == aubo_robot_namespace::InterfaceCallSuccCode) {
        std::cout << "login successful." << std::endl;
    } else {
        std::cerr << "login failed." << std::endl;
    }
    aubo_robot_namespace::RobotSafetyConfig safetyConfig;
    ret = robotService.robotServiceSetRobotSafetyConfig(safetyConfig);
}
