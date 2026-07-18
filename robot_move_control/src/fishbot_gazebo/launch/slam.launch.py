from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    use_sim_time = LaunchConfiguration('use_sim_time', default='true')
    fishbot_share = FindPackageShare('fishbot_gazebo')
    simulation = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(PathJoinSubstitution([fishbot_share, 'launch', 'simulation.launch.py'])),
        launch_arguments={'use_sim_time': use_sim_time}.items(),
    )
    slam = Node(
        package='slam_toolbox', executable='async_slam_toolbox_node', name='slam_toolbox', output='screen',
        parameters=[{'use_sim_time': use_sim_time, 'base_frame': 'base_footprint', 'odom_frame': 'odom', 'scan_topic': '/scan'}],
    )
    return LaunchDescription([simulation, slam])
