from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import Command, FindExecutable, LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    model = LaunchConfiguration("model")
    robot_description = ParameterValue(
        Command([FindExecutable(name="xacro"), " ", model]), value_type=str
    )
    return LaunchDescription([
        DeclareLaunchArgument(
            "model",
            default_value=PathJoinSubstitution(
                [FindPackageShare("wheeltec_description"), "urdf", "wheeltec_mini_mec.urdf.xacro"]
            ),
        ),
        Node(
            package="joint_state_publisher_gui",
            executable="joint_state_publisher_gui",
            output="screen",
        ),
        Node(
            package="robot_state_publisher",
            executable="robot_state_publisher",
            parameters=[{"robot_description": robot_description}],
            output="screen",
        ),
        Node(package="rviz2", executable="rviz2", output="screen"),
    ])
