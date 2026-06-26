from launch import LaunchDescription
from launch_ros.actions import Node
import os
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    
    package_dir = get_package_share_directory('robot_control_ros2')
    config_dir = os.path.join(
        package_dir,
        'config',
        'vel_to_pos.yaml'
    )

    v2p_node = Node(package="robot_control_ros2",
                executable="vel_to_pos_node",
                exec_name="v2p_node",
                output='screen',
                parameters=[config_dir],
                respawn=True)
    vPub_node = Node(package="robot_control_ros2",
                executable="robot_velocity_pub",
                exec_name="vPub_node",
                output='screen',
                parameters=[config_dir],
                respawn=True)
    
    return LaunchDescription([v2p_node, vPub_node])