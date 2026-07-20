from pathlib import Path

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    config = Path(get_package_share_directory("wheeltec_base_driver")) / "config" / "wheeltec_base_driver.yaml"

    return LaunchDescription([
        Node(
            package="wheeltec_base_driver",
            executable="wheeltec_base_driver_node",
            name="wheeltec_base_driver",
            output="screen",
            parameters=[str(config)],
        )
    ])
