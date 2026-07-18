"""Start the safety-gated AUBO hardware and a MoveIt planning client.

This launch deliberately delegates all hardware startup to aubo_i5_real.launch.py.
The trajectory controller is only spawned when enable_motion:=true, so MoveIt
can be used for visualization and planning during the read-only acceptance step.
"""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    robot_ip = LaunchConfiguration("robot_ip")
    enable_motion = LaunchConfiguration("enable_motion")
    hardware_share = FindPackageShare("aubo_i5_hardware")
    moveit_share = FindPackageShare("aubo_i5_moveit_config")

    hardware = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([hardware_share, "launch", "aubo_i5_real.launch.py"])
        ),
        launch_arguments={
            "robot_ip": robot_ip,
            "enable_motion": enable_motion,
        }.items(),
    )
    # These launch files start planning and RViz only.  They do not create a
    # second controller_manager, which must remain owned by the hardware launch.
    move_group = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([moveit_share, "launch", "move_group.launch.py"])
        )
    )
    moveit_rviz = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([moveit_share, "launch", "moveit_rviz.launch.py"])
        )
    )

    return LaunchDescription([
        DeclareLaunchArgument(
            "robot_ip",
            default_value="",
            description="Required AUBO controller IP; no real address is stored in the workspace.",
        ),
        DeclareLaunchArgument(
            "enable_motion",
            default_value="false",
            description="Explicitly permit Servo writes and the trajectory controller.",
        ),
        hardware,
        move_group,
        moveit_rviz,
    ])
