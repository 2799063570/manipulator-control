from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import Command, FindExecutable, LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    use_sim_time = LaunchConfiguration("use_sim_time")
    world = LaunchConfiguration("world")
    gazebo_share = FindPackageShare("wheeltec_gazebo")
    model = PathJoinSubstitution([gazebo_share, "urdf", "wheeltec_mini_mec.gazebo.urdf.xacro"])
    robot_description = ParameterValue(
        Command([FindExecutable(name="xacro"), " ", model]), value_type=str
    )

    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([FindPackageShare("ros_gz_sim"), "launch", "gz_sim.launch.py"])
        ),
        launch_arguments={"gz_args": ["-r -v 4 ", world]}.items(),
    )
    bridge = Node(
        package="ros_gz_bridge",
        executable="parameter_bridge",
        output="screen",
        arguments=[
            "/world/wheeltec_world/clock@rosgraph_msgs/msg/Clock[gz.msgs.Clock",
            "/cmd_vel@geometry_msgs/msg/Twist]gz.msgs.Twist",
            "/odom@nav_msgs/msg/Odometry[gz.msgs.Odometry",
            "/tf@tf2_msgs/msg/TFMessage[gz.msgs.Pose_V",
            "/scan@sensor_msgs/msg/LaserScan[gz.msgs.LaserScan",
        ],
        remappings=[("/world/wheeltec_world/clock", "/clock")],
    )
    state_publisher = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        output="screen",
        parameters=[{"robot_description": robot_description, "use_sim_time": use_sim_time}],
    )
    spawn_robot = Node(
        package="ros_gz_sim",
        executable="create",
        output="screen",
        arguments=["-name", "wheeltec_mini_mec", "-topic", "robot_description", "-x", "-2.5", "-y", "1.5", "-z", "0.08"],
    )

    return LaunchDescription([
        DeclareLaunchArgument("use_sim_time", default_value="true"),
        DeclareLaunchArgument(
            "world",
            default_value=PathJoinSubstitution([gazebo_share, "worlds", "wheeltec_world.sdf"]),
        ),
        gazebo,
        bridge,
        state_publisher,
        spawn_robot,
    ])
