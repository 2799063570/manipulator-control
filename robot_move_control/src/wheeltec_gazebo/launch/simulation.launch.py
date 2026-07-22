from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import Command, FindExecutable, LaunchConfiguration, PathJoinSubstitution, PythonExpression
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    use_sim_time = LaunchConfiguration("use_sim_time")
    navigation_enabled = LaunchConfiguration("navigation_enabled")
    gazebo_gui = LaunchConfiguration("gazebo_gui")
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
        # Running the Gazebo GUI alongside RViz and a GPU lidar can exhaust the
        # graphics driver on modest machines.  Keep it optional; the server,
        # sensors and ROS topics still run with gazebo_gui:=false.
        launch_arguments={
            "gz_args": [
                PythonExpression(["'-r -v 4 ' if '", gazebo_gui, "' == 'true' else '-r -v 4 -s '"]),
                world,
            ]
        }.items(),
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
            "/joint_states@sensor_msgs/msg/JointState[gz.msgs.Model",
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
    command_mux = Node(
        package="wheeltec_control",
        executable="cmd_mux",
        output="screen",
        parameters=[{"navigation_enabled": navigation_enabled}],
    )
    spawn_robot = Node(
        package="ros_gz_sim",
        executable="create",
        output="screen",
        # The saved wheeltec_lab map has its (0, 0, 0) pose at this location
        # in wheeltec_world.  Keeping this fixed lets AMCL's automatic initial
        # pose (0, 0, 0) align Gazebo, RViz and the saved map on every launch.
        # base_footprint is on the ground plane.  Spawning it at z=0.08 left
        # the wheel contact patches floating 8 cm above the floor: encoders
        # turned while the physical robot (and lidar) stayed still.
        arguments=["-name", "wheeltec_mini_mec", "-topic", "robot_description", "-x", "-2.484", "-y", "1.520", "-z", "0.0"],
    )

    return LaunchDescription([
        DeclareLaunchArgument("use_sim_time", default_value="true"),
        DeclareLaunchArgument(
            "navigation_enabled",
            default_value="false",
            description="Accept Nav2 commands from /cmd_vel_nav.",
        ),
        DeclareLaunchArgument(
            "gazebo_gui",
            default_value="true",
            description="Start the Gazebo graphical client. Set false for headless simulation.",
        ),
        DeclareLaunchArgument(
            "world",
            default_value=PathJoinSubstitution([gazebo_share, "worlds", "wheeltec_world.sdf"]),
        ),
        gazebo,
        bridge,
        state_publisher,
        command_mux,
        spawn_robot,
    ])
