from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    localization = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([FindPackageShare("nav2_bringup"), "launch", "localization_launch.py"])
        ),
        launch_arguments={
            "map": LaunchConfiguration("map_file"),
            "use_sim_time": LaunchConfiguration("use_sim_time"),
            "autostart": LaunchConfiguration("autostart"),
            "params_file": LaunchConfiguration("params_file"),
            "use_composition": "False",
        }.items(),
    )

    return LaunchDescription([
        DeclareLaunchArgument(
            "map_file", description="Absolute path to the saved map YAML file.",
        ),
        DeclareLaunchArgument("use_sim_time", default_value="false"),
        DeclareLaunchArgument("autostart", default_value="true"),
        DeclareLaunchArgument(
            "params_file",
            default_value=PathJoinSubstitution(
                [FindPackageShare("wheeltec_navigation"), "config", "nav2_params.yaml"]
            ),
        ),
        localization,
    ])
