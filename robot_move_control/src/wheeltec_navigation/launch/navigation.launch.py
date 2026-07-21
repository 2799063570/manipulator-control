from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, LogInfo, OpaqueFunction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare


def _launch_nav2(context, *args, **kwargs):
    """Resolve parent launch arguments before entering Nav2's map scope."""
    map_file = LaunchConfiguration("map").perform(context)
    if not map_file:
        raise RuntimeError("A saved map is required. Pass map:=/absolute/path/to/map.yaml")
    nav2_bringup = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([FindPackageShare("nav2_bringup"), "launch", "bringup_launch.py"])
        ),
        launch_arguments={
            "map": map_file,
            "use_sim_time": LaunchConfiguration("use_sim_time").perform(context),
            "autostart": LaunchConfiguration("autostart").perform(context),
            "params_file": LaunchConfiguration("params_file").perform(context),
            "use_composition": "False",
        }.items(),
    )

    return [
        LogInfo(msg=["Starting Nav2 with map: ", map_file]),
        nav2_bringup,
    ]


def generate_launch_description():
    return LaunchDescription([
        DeclareLaunchArgument(
            "map", description="Absolute path to the saved map YAML file.",
        ),
        DeclareLaunchArgument("use_sim_time", default_value="false"),
        DeclareLaunchArgument("autostart", default_value="true"),
        DeclareLaunchArgument(
            "params_file",
            default_value=PathJoinSubstitution(
                [FindPackageShare("wheeltec_navigation"), "config", "nav2_params.yaml"]
            ),
        ),
        OpaqueFunction(function=_launch_nav2),
    ])
