from launch import LaunchDescription
from launch_ros.actions import Node
import os
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    

    node = Node(package="robot_control_ros2",
                executable="tf_static_broadcaster",
                name="tf_static_broadcaster",
                output='screen',
                arguments=["1", "1", "1", "1", "1", "1", "laser", "camera"],
                respawn=True)

    
    return LaunchDescription([node])