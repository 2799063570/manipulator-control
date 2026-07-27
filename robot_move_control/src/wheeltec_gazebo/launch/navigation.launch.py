from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, LogInfo, OpaqueFunction, TimerAction
from launch.conditions import IfCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def _launch_navigation(context, *args, **kwargs):
    """Resolve the outer map argument before the child launch declares map."""
    map_file = LaunchConfiguration("map_file").perform(context)
    if not map_file:
        raise RuntimeError("A saved map is required. Pass map_file:=/absolute/path/to/map.yaml")
    use_sim_time = LaunchConfiguration("use_sim_time").perform(context)

    return [
        LogInfo(msg=["Launching Wheeltec navigation with map: ", map_file]),
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(
                PathJoinSubstitution([FindPackageShare("wheeltec_navigation"), "launch", "navigation.launch.py"])
            ),
            launch_arguments={
                "use_sim_time": use_sim_time,
                "map_file": map_file,
                # This simulation always spawns at the saved map's (0, 0, 0).
                # Real-robot bringup keeps this false and waits for /initialpose.
                "set_initial_pose": "true",
                "initial_pose_x": "0.0",
                "initial_pose_y": "0.0",
                "initial_pose_yaw": "0.0",
            }.items(),
        ),
    ]


def generate_launch_description():
    use_sim_time = LaunchConfiguration("use_sim_time")
    use_rviz = LaunchConfiguration("use_rviz")
    gazebo_gui = LaunchConfiguration("gazebo_gui")
    nav_start_delay = LaunchConfiguration("nav_start_delay")
    gz_partition = LaunchConfiguration("gz_partition")
    instance_lock_file = LaunchConfiguration("instance_lock_file")
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([FindPackageShare("wheeltec_gazebo"), "launch", "simulation.launch.py"])
        ),
        launch_arguments={
            "use_sim_time": use_sim_time,
            "navigation_enabled": "true",
            "gazebo_gui": gazebo_gui,
            "gz_partition": gz_partition,
            "instance_lock_file": instance_lock_file,
        }.items(),
    )
    rviz = Node(
        package="rviz2",
        executable="rviz2",
        output="screen",
        arguments=["-d", PathJoinSubstitution([FindPackageShare("wheeltec_gazebo"), "config", "navigation.rviz"])],
        parameters=[{"use_sim_time": use_sim_time}],
        condition=IfCondition(use_rviz),
    )
    return LaunchDescription([
        DeclareLaunchArgument("use_sim_time", default_value="true"),
        DeclareLaunchArgument("map_file", description="Absolute path to the map YAML file."),
        DeclareLaunchArgument("use_rviz", default_value="true", description="Start RViz with the Wheeltec map configuration."),
        DeclareLaunchArgument("gazebo_gui", default_value="true", description="Start Gazebo GUI; use false if it freezes."),
        DeclareLaunchArgument(
            "nav_start_delay",
            default_value="5.0",
            description="Wall-clock delay before Nav2 starts, allowing Gazebo odom/scan/TF to become available.",
        ),
        DeclareLaunchArgument(
            "gz_partition",
            default_value="wheeltec_robot_move_control",
            description="Gazebo Transport partition used to isolate this simulation from unrelated Gazebo instances.",
        ),
        DeclareLaunchArgument(
            "instance_lock_file",
            default_value="/tmp/wheeltec_world.lock",
            description="Process lock preventing multiple wheeltec_world simulations from sharing /clock.",
        ),
        gazebo,
        TimerAction(period=nav_start_delay, actions=[OpaqueFunction(function=_launch_navigation)]),
        rviz,
    ])
