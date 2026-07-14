#include "util.h"

#include <thread>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <fstream>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
// Print waypoint information
void Util::printWaypoint(aubo_robot_namespace::wayPoint_S &wayPoint)
{
    std::cout << std::endl
              << "start-------------waypoint---------------" << std::endl;
    // position information
    std::cout << "position information: ";
    std::cout << "x:" << wayPoint.cartPos.position.x << "  ";
    std::cout << "y:" << wayPoint.cartPos.position.y << "  ";
    std::cout << "z:" << wayPoint.cartPos.position.z << std::endl;

    // Gesture information
    std::cout << "Gesture information: ";
    std::cout << "w:" << wayPoint.orientation.w << "  ";
    std::cout << "x:" << wayPoint.orientation.x << "  ";
    std::cout << "y:" << wayPoint.orientation.y << "  ";
    std::cout << "z:" << wayPoint.orientation.z << std::endl;

    //    aubo_robot_namespace::Rpy tempRpy;
    //    robotService.quaternionToRPY(wayPoint.orientation,tempRpy);
    //    std::cout<<"RX:"<<tempRpy.rx<<"  RY:"<<tempRpy.ry<<"
    //    RZ:"<<tempRpy.rz<<std::endl;

    // Joint information
    std::cout << "Joint information: " << std::endl;
    for (int i = 0; i < aubo_robot_namespace::ARM_DOF; i++) {
        std::cout << "joint" << i + 1 << ": " << wayPoint.jointpos[i] << " ~ "
                  << wayPoint.jointpos[i] * 180.0 / M_PI << std::endl;
    }
}

// Print joint status information
void Util::printJointStatus(
    const aubo_robot_namespace::JointStatus *jointStatus, int len)
{
    std::cout << std::endl
              << "start----------Joint status information-------" << std::endl;

    for (int i = 0; i < len; i++) {
        std::cout << "Joint ID:" << i << "  ";
        std::cout << "Current:" << jointStatus[i].jointCurrentI << " ";
        std::cout << "speed:" << jointStatus[i].jointSpeedMoto << " ";
        std::cout << "Joint angle:" << jointStatus[i].jointPosJ << " "
                  << " ~ " << jointStatus[i].jointPosJ * 180.0 / M_PI;
        std::cout << "Voltage   :" << jointStatus[i].jointCurVol << " ";
        std::cout << "Temperature   :" << jointStatus[i].jointCurTemp << " ";
        std::cout << "Target current:" << jointStatus[i].jointTagCurrentI
                  << " ";
        std::cout << "Target motor speed:" << jointStatus[i].jointTagSpeedMoto
                  << " ";
        std::cout << "Target joint angle :" << jointStatus[i].jointTagPosJ
                  << " ";
        std::cout << "Joint error   :" << jointStatus[i].jointErrorNum
                  << std::endl;
    }
    std::cout << std::endl;
}

// Print event information
void Util::printEventInfo(const aubo_robot_namespace::RobotEventInfo &eventInfo)
{
    std::cout << "Event type:" << eventInfo.eventType
              << "  code:" << eventInfo.eventCode
              << "  Content:" << eventInfo.eventContent << std::endl;
}

void Util::printRobotDiagnosis(
    const aubo_robot_namespace::RobotDiagnosis &robotDiagnosis)
{
    std::cout << std::endl
              << "start----------Robotic arm statistics-------" << std::endl;

    std::cout << std::endl
              << "   "
              << "CAN communication status:"
              << (int)robotDiagnosis.armCanbusStatus;
    std::cout << std::endl
              << "   "
              << "Current current of the power supply:"
              << robotDiagnosis.armPowerCurrent;
    std::cout << std::endl
              << "   "
              << "Current voltage of the power supply:"
              << robotDiagnosis.armPowerVoltage;

    (robotDiagnosis.armPowerStatus) ? std::cout << std::endl
                                                << "   "
                                                << "48V power status: On"
                                    : std::cout << std::endl
                                                << "   "
                                                << "48V power status: Off";

    std::cout << std::endl
              << "   "
              << "Control box temperature:"
              << (int)robotDiagnosis.contorllerTemp;
    std::cout << std::endl
              << "   "
              << "Control box humidity:"
              << (int)robotDiagnosis.contorllerHumidity;
    std::cout << std::endl
              << "   "
              << "Remote shutdown signal:" << robotDiagnosis.remoteHalt;
    std::cout << std::endl
              << "   "
              << "Robot arm soft stop:" << robotDiagnosis.softEmergency;
    std::cout << std::endl
              << "   "
              << "Remote emergency stop signal:"
              << robotDiagnosis.remoteEmergency;
    std::cout << std::endl
              << "   "
              << "Collision detection bit:" << robotDiagnosis.robotCollision;
    std::cout << std::endl
              << "   "
              << "Enter the force control mode flag:"
              << robotDiagnosis.forceControlMode;
    std::cout << std::endl
              << "   "
              << "Brake status:" << robotDiagnosis.brakeStuats;
    std::cout << std::endl
              << "   "
              << "End speed:" << robotDiagnosis.robotEndSpeed;
    std::cout << std::endl
              << "   "
              << "Maximum acceleration:" << robotDiagnosis.robotMaxAcc;
    std::cout << std::endl
              << "   "
              << "Host computer software status bit:"
              << robotDiagnosis.orpeStatus;
    std::cout << std::endl
              << "   "
              << "Pose reading enable bit:" << robotDiagnosis.enableReadPose;
    std::cout << std::endl
              << "   "
              << "Installation location status:"
              << robotDiagnosis.robotMountingPoseChanged;
    std::cout << std::endl
              << "   "
              << "Magnetic encoder error status:"
              << robotDiagnosis.encoderErrorStatus;
    std::cout << std::endl
              << "   "
              << "Static collision detection switch:"
              << robotDiagnosis.staticCollisionDetect;
    std::cout << std::endl
              << "   "
              << "Joint collision detection:"
              << robotDiagnosis.jointCollisionDetect;
    std::cout << std::endl
              << "   "
              << "Optical encoder inconsistency error:"
              << robotDiagnosis.encoderLinesError;
    std::cout << std::endl
              << "   "
              << "Joint error status:" << robotDiagnosis.jointErrorStatus;
    std::cout << std::endl
              << "   "
              << "Singular point overspeed warning:"
              << robotDiagnosis.singularityOverSpeedAlarm;
    std::cout << std::endl
              << "   "
              << "Current error warning:" << robotDiagnosis.robotCurrentAlarm;
    std::cout << std::endl
              << "   "
              << "Tool error:" << (int)robotDiagnosis.toolIoError;
    std::cout << std::endl
              << "   "
              << "Misplacement of installation position:"
              << robotDiagnosis.robotMountingPoseWarning;
    std::cout << std::endl
              << "   "
              << "Mac buffer length:" << robotDiagnosis.macTargetPosBufferSize;
    std::cout << std::endl
              << "   "
              << "Mac buffer valid data length:"
              << robotDiagnosis.macTargetPosDataSize;
    std::cout << std::endl
              << "   "
              << "Mac data interruption:"
              << robotDiagnosis.macDataInterruptWarning;

    std::cout << std::endl
              << "----------------------------------end." << std::endl;
}

void Util::initJointAngleArray(double *array, double joint0, double joint1,
                               double joint2, double joint3, double joint4,
                               double joint5)
{
    array[0] = joint0;
    array[1] = joint1;
    array[2] = joint2;
    array[3] = joint3;
    array[4] = joint4;
    array[5] = joint5;
}
