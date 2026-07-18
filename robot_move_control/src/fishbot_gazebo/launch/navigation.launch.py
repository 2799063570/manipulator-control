from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, GroupAction, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import SetRemap
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    fishbot_share = FindPackageShare('fishbot_gazebo')
    nav2_share = FindPackageShare('nav2_bringup')
    use_sim_time = LaunchConfiguration('use_sim_time')
    map_file = LaunchConfiguration('map')
    simulation = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(PathJoinSubstitution([fishbot_share, 'launch', 'simulation.launch.py'])),
        launch_arguments={'use_sim_time': use_sim_time}.items(),
    )
    nav2 = GroupAction([
        # Apply the remap to every Nav2 node, keeping its internal velocity
        # pipeline intact while exposing a single mux input to Fishbot.
        SetRemap(src='cmd_vel', dst='cmd_vel_nav'),
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(PathJoinSubstitution([nav2_share, 'launch', 'bringup_launch.py'])),
            launch_arguments={
                'map': map_file,
                'use_sim_time': use_sim_time,
                'autostart': 'true',
                'params_file': PathJoinSubstitution([nav2_share, 'params', 'nav2_params.yaml']),
            }.items(),
        ),
    ])
    )
    return LaunchDescription([
        DeclareLaunchArgument('use_sim_time', default_value='true'),
        DeclareLaunchArgument('map', description='Absolute path to the map YAML saved by slam_toolbox.'),
        simulation, nav2,
    ])
