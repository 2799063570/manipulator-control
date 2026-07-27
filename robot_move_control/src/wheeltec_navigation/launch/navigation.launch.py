"""Minimal Nav2 bringup used by the Wheeltec Gazebo teaching example.

The standard Jazzy bringup also starts routing, docking and collision-monitor
servers.  They are useful in a complete robot stack, but obscure the basic
navigation signal flow while learning.  This launch file deliberately keeps a
small, inspectable set of nodes.
"""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, LogInfo, OpaqueFunction, TimerAction
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def _as_bool(value: str) -> bool:
    return value.lower() in ("1", "true", "yes", "on")


def _launch_nav2(context, *args, **kwargs):
    map_file = LaunchConfiguration("map_file").perform(context)
    if not map_file:
        raise RuntimeError("A saved map is required. Pass map_file:=/absolute/path/to/map.yaml")

    params_file = LaunchConfiguration("params_file").perform(context)
    use_sim_time = _as_bool(LaunchConfiguration("use_sim_time").perform(context))
    set_initial_pose = _as_bool(LaunchConfiguration("set_initial_pose").perform(context))
    initial_pose = {
        "x": float(LaunchConfiguration("initial_pose_x").perform(context)),
        "y": float(LaunchConfiguration("initial_pose_y").perform(context)),
        "z": 0.0,
        "yaw": float(LaunchConfiguration("initial_pose_yaw").perform(context)),
    }
    common = [params_file, {"use_sim_time": use_sim_time}]

    localization_nodes = ["map_server", "amcl"]
    navigation_nodes = [
        "controller_server",
        "planner_server",
        "behavior_server",
        "velocity_smoother",
        "bt_navigator",
    ]

    return [
        LogInfo(msg=["Starting minimal Wheeltec Nav2 with map: ", map_file]),
        # Localization: map_server publishes the saved map and AMCL owns
        # map -> odom.  No mapping node must run at the same time.
        Node(
            package="nav2_map_server",
            executable="map_server",
            name="map_server",
            output="screen",
            parameters=[params_file, {"use_sim_time": use_sim_time, "yaml_filename": map_file}],
        ),
        Node(
            package="nav2_amcl",
            executable="amcl",
            name="amcl",
            output="screen",
            parameters=[
                params_file,
                {
                    "use_sim_time": use_sim_time,
                    "set_initial_pose": set_initial_pose,
                    "initial_pose": initial_pose,
                },
            ],
        ),
        Node(
            package="nav2_lifecycle_manager",
            executable="lifecycle_manager",
            name="lifecycle_manager_localization",
            output="screen",
            parameters=[{"use_sim_time": use_sim_time, "autostart": True, "node_names": localization_nodes}],
        ),
        # Navigation velocity pipeline:
        # controller/behaviors -> /cmd_vel_nav -> velocity_smoother
        # -> /cmd_vel_smoothed -> wheeltec_cmd_mux -> /cmd_vel -> Gazebo.
        Node(
            package="nav2_controller",
            executable="controller_server",
            name="controller_server",
            output="screen",
            parameters=common,
            remappings=[("cmd_vel", "/cmd_vel_nav")],
        ),
        Node(
            package="nav2_planner",
            executable="planner_server",
            name="planner_server",
            output="screen",
            parameters=common,
        ),
        Node(
            package="nav2_behaviors",
            executable="behavior_server",
            name="behavior_server",
            output="screen",
            parameters=common,
            remappings=[("cmd_vel", "/cmd_vel_nav")],
        ),
        Node(
            package="nav2_velocity_smoother",
            executable="velocity_smoother",
            name="velocity_smoother",
            output="screen",
            parameters=common,
            remappings=[("cmd_vel", "/cmd_vel_nav")],
        ),
        Node(
            package="nav2_bt_navigator",
            executable="bt_navigator",
            name="bt_navigator",
            output="screen",
            parameters=common,
        ),
        # Let map_server and AMCL finish their lifecycle transitions first.
        # The retrying helper treats a slow lifecycle service response as
        # recoverable if the node reaches the requested state afterwards.
        TimerAction(
            period=2.0,
            actions=[
                Node(
                    package="wheeltec_navigation",
                    executable="nav2_lifecycle_bringup.py",
                    name="wheeltec_nav2_lifecycle_bringup",
                    output="screen",
                    arguments=["--nodes", *navigation_nodes],
                    parameters=[{"use_sim_time": use_sim_time}],
                )
            ],
        ),
    ]


def generate_launch_description():
    return LaunchDescription([
        DeclareLaunchArgument("map_file", description="Absolute path to the saved map YAML file."),
        DeclareLaunchArgument("use_sim_time", default_value="true"),
        DeclareLaunchArgument(
            "set_initial_pose",
            default_value="false",
            description="Let AMCL initialize from initial_pose_* instead of waiting for /initialpose.",
        ),
        DeclareLaunchArgument("initial_pose_x", default_value="0.0"),
        DeclareLaunchArgument("initial_pose_y", default_value="0.0"),
        DeclareLaunchArgument("initial_pose_yaw", default_value="0.0"),
        DeclareLaunchArgument(
            "params_file",
            default_value=PathJoinSubstitution(
                [FindPackageShare("wheeltec_navigation"), "config", "nav2_params.yaml"]
            ),
        ),
        OpaqueFunction(function=_launch_nav2),
    ])
