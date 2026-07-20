from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    use_sim_time = LaunchConfiguration("use_sim_time")
    map_file = LaunchConfiguration("map")
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([FindPackageShare("wheeltec_gazebo"), "launch", "simulation.launch.py"])
        ),
        launch_arguments={"use_sim_time": use_sim_time}.items(),
    )
    navigation = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([FindPackageShare("wheeltec_navigation"), "launch", "navigation.launch.py"])
        ),
        launch_arguments={"use_sim_time": use_sim_time, "map": map_file}.items(),
    )
    return LaunchDescription([
        DeclareLaunchArgument("use_sim_time", default_value="true"),
        DeclareLaunchArgument("map", description="Absolute path to the map YAML file."),
        gazebo,
        navigation,
    ])
