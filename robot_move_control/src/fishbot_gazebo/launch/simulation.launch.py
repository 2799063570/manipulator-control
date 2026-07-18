from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, TimerAction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import Command, FindExecutable, LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    use_sim_time = LaunchConfiguration('use_sim_time')
    fishbot_share = FindPackageShare('fishbot_gazebo')
    world = LaunchConfiguration('world')
    xacro_file = PathJoinSubstitution([fishbot_share, 'urdf', 'fishbot.urdf.xacro'])
    robot_description = {
        'robot_description': ParameterValue(
            Command([FindExecutable(name='xacro'), ' ', xacro_file]), value_type=str
        )
    }

    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(PathJoinSubstitution([
            FindPackageShare('ros_gz_sim'), 'launch', 'gz_sim.launch.py'
        ])),
        launch_arguments={'gz_args': ['-r -v 4 ', world]}.items(),
    )
    clock_bridge = Node(
        package='ros_gz_bridge', executable='parameter_bridge', output='screen',
        arguments=['/world/nav2_world/clock@rosgraph_msgs/msg/Clock[gz.msgs.Clock'],
        remappings=[('/world/nav2_world/clock', '/clock')],
    )
    scan_bridge = Node(
        package='ros_gz_bridge', executable='parameter_bridge', output='screen',
        arguments=['/scan@sensor_msgs/msg/LaserScan[gz.msgs.LaserScan'],
    )
    state_publisher = Node(
        package='robot_state_publisher', executable='robot_state_publisher', output='screen',
        parameters=[robot_description, {'use_sim_time': use_sim_time}],
    )
    spawn_robot = Node(
        package='ros_gz_sim', executable='create', output='screen',
        arguments=['-name', 'fishbot', '-topic', 'robot_description', '-x', '-2.5', '-y', '1.5', '-z', '0.05'],
    )
    state_broadcaster = Node(
        package='controller_manager', executable='spawner', output='screen',
        arguments=['joint_state_broadcaster', '--controller-manager', '/controller_manager'],
    )
    base_controller = Node(
        package='controller_manager', executable='spawner', output='screen',
        arguments=['base_controller', '--controller-manager', '/controller_manager'],
    )
    twist_mux = Node(
        package='twist_mux', executable='twist_mux', output='screen',
        parameters=[PathJoinSubstitution([fishbot_share, 'config', 'twist_mux.yaml']), {'use_sim_time': use_sim_time}],
    )

    return LaunchDescription([
        DeclareLaunchArgument('use_sim_time', default_value='true'),
        DeclareLaunchArgument('world', default_value=PathJoinSubstitution([fishbot_share, 'worlds', 'nav2_world.sdf'])),
        gazebo, clock_bridge, scan_bridge, state_publisher, spawn_robot,
        TimerAction(period=5.0, actions=[state_broadcaster, base_controller]), twist_mux,
    ])
