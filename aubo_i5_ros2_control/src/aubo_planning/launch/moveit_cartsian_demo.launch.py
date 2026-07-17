import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
from moveit_configs_utils import MoveItConfigsBuilder

def generate_launch_description():
    # 替换成你实际的 Aubo i5 MoveIt 配置包名
    moveit_config = (
        MoveItConfigsBuilder("aubo_i5", package_name="aubo_i5_moveit_config")
        .robot_description(file_path="config/aubo_i5.urdf.xacro")
        .robot_description_semantic(file_path="config/aubo_i5.srdf")
        .robot_description_kinematics(file_path="config/kinematics.yaml")
        .to_moveit_configs()
    )

    moveit_cartesian_demo = Node(
        package="aubo_planning",
        executable="moveit_cartesian_demo",
        output="screen",
        # 核心：把机器人描述参数全部传入你的节点
        parameters=[
            moveit_config.robot_description,
            moveit_config.robot_description_semantic,
            moveit_config.robot_description_kinematics,
        ],
    )

    return LaunchDescription([moveit_cartesian_demo])