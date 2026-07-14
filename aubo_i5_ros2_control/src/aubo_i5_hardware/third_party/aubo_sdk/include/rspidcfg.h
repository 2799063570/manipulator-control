#ifndef RSPIDCFG_H
#define RSPIDCFG_H
#include "rstype.h"
#include "AuboRobotMetaType.h"

using namespace aubo_robot_namespace;

typedef struct
{
    JointCommonData data[ARM_DOF];
} RobotJointCommonData;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 获取机械臂配置表
 * @param rshd
 * @param data 六个关节配置数据
 * @return
 */
int rs_get_joint_common_data(RSHD rshd, RobotJointCommonData &data);

/**
 * @brief 设置机械臂电流环参数P
 * @param joint_id 机械臂ID（1~6)
 * @param P        电流环参数P
 * @return
 */
int rs_set_current_ip(RSHD rshd, int joint_id, uint16 P);

/**
 * @brief 设置机械臂电流环参数I
 * @param joint_id 机械臂ID（1~6)
 * @param I        电流环参数I
 * @return
 */
int rs_set_current_ii(RSHD rshd, int joint_id, uint16 I);

/**
 * @brief 设置机械臂电流环参数D
 * @param joint_id 机械臂ID（1~6)
 * @param D        电流环参数D
 * @return
 */
int rs_set_current_id(RSHD rshd, int joint_id, uint16 D);

/**
 * @brief 设置机械臂速度环参数P
 * @param joint_id 机械臂ID（1~6)
 * @param P        速度环参数P
 * @return
 */
int rs_set_speed_p(RSHD rshd, int joint_id, uint16 P);

/**
 * @brief 设置机械臂速度环参数I
 * @param joint_id 机械臂ID（1~6)
 * @param I        速度环参数I
 * @return
 */
int rs_set_speed_i(RSHD rshd, int joint_id, uint16 I);

/**
 * @brief 设置机械臂速度环参数D
 * @param joint_id 机械臂ID（1~6)
 * @param D        速度环参数D
 * @return
 */
int rs_set_speed_d(RSHD rshd, int joint_id, uint16 D);

/**
 * @brief 设置机械臂速度环参数DS
 * @param joint_id 机械臂ID（1~6)
 * @param DS       速度环参数DS
 * @return
 */
int rs_set_speed_ds(RSHD rshd, int joint_id, uint16 DS);

/**
 * @brief 设置机械臂位置环参数P
 * @param joint_id 机械臂ID（1~6)
 * @param P       位置环参数P
 * @return
 */
int rs_set_pos_p(RSHD rshd, int joint_id, uint16 P);

/**
 * @brief 设置机械臂位置环参数I
 * @param joint_id 机械臂ID（1~6)
 * @param I        位置环参数I
 * @return
 */
int rs_set_pos_i(RSHD rshd, int joint_id, uint16 I);

/**
 * @brief 设置机械臂位置环参数D
 * @param joint_id 机械臂ID（1~6)
 * @param D        位置环参数D
 * @return
 */
int rs_set_pos_d(RSHD rshd, int joint_id, uint16 D);

/**
 * @brief 设置机械臂位置环参数DS
 * @param joint_id 机械臂ID（1~6)
 * @param DS       位置环参数DS
 * @return
 */
int rs_set_pos_ds(RSHD rshd, int joint_id, uint16 DS);

/**
 * @brief 保存机械臂PID参数到关节flash
 * @param rshd
 * @param joint_id
 * @return
 */
int rs_joint_save_data_flash(RSHD rshd, int joint_id);

#ifdef __cplusplus
}
#endif

#endif // RSPIDCFG_H
