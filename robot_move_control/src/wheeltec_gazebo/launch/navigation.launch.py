from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, LogInfo, OpaqueFunction
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
            launch_arguments={"use_sim_time": use_sim_time, "map_file": map_file}.items(),
        ),
    ]


def generate_launch_description():
    use_sim_time = LaunchConfiguration("use_sim_time")
    use_rviz = LaunchConfiguration("use_rviz")
    gazebo_gui = LaunchConfiguration("gazebo_gui")
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([FindPackageShare("wheeltec_gazebo"), "launch", "simulation.launch.py"])
        ),
        launch_arguments={
            "use_sim_time": use_sim_time,
            "navigation_enabled": "true",
            "gazebo_gui": gazebo_gui,
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
        gazebo,
        OpaqueFunction(function=_launch_navigation),
        rviz,
    ])
