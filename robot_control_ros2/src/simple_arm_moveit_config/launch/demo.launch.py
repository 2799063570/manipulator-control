from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    planning_execution_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution(
                [
                    FindPackageShare("simple_arm_moveit_config"),
                    "launch",
                    "planning_execution.launch.py",
                ]
            )
        )
    )

    return LaunchDescription([planning_execution_launch])
