from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, GroupAction, IncludeLaunchDescription, OpaqueFunction
from launch.conditions import IfCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node, SetRemap
from launch_ros.substitutions import FindPackageShare


def _launch_navigation(context, *args, **kwargs):
    """Resolve the outer map argument before the child launch declares map."""
    map_file = LaunchConfiguration("map").perform(context)
    if not map_file:
        raise RuntimeError("A saved map is required. Pass map:=/absolute/path/to/map.yaml")
    use_sim_time = LaunchConfiguration("use_sim_time").perform(context)

    return [
        GroupAction([
            # Keep Nav2 separate from manual teleoperation.  The command mux then
            # selects /cmd_vel_nav unless the operator is actively commanding.
            SetRemap(src="cmd_vel", dst="cmd_vel_nav"),
            IncludeLaunchDescription(
                PythonLaunchDescriptionSource(
                    PathJoinSubstitution([FindPackageShare("wheeltec_navigation"), "launch", "navigation.launch.py"])
                ),
                launch_arguments={"use_sim_time": use_sim_time, "map": map_file}.items(),
            ),
        ])
    ]


def generate_launch_description():
    use_sim_time = LaunchConfiguration("use_sim_time")
    use_rviz = LaunchConfiguration("use_rviz")
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([FindPackageShare("wheeltec_gazebo"), "launch", "simulation.launch.py"])
        ),
        launch_arguments={
            "use_sim_time": use_sim_time,
            "navigation_enabled": "true",
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
        DeclareLaunchArgument("map", description="Absolute path to the map YAML file."),
        DeclareLaunchArgument("use_rviz", default_value="true", description="Start RViz with the Wheeltec map configuration."),
        gazebo,
        OpaqueFunction(function=_launch_navigation),
        rviz,
    ])
