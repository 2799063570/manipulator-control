import fcntl
import os

from launch import LaunchDescription
from launch.actions import (
    DeclareLaunchArgument,
    IncludeLaunchDescription,
    OpaqueFunction,
    SetEnvironmentVariable,
)
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import Command, FindExecutable, LaunchConfiguration, PathJoinSubstitution, PythonExpression
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
from launch_ros.substitutions import FindPackageShare


_instance_lock = None


def _acquire_instance_lock(context, *args, **kwargs):
    """Prevent two wheeltec_world stacks from publishing competing clocks."""
    global _instance_lock
    lock_file = LaunchConfiguration("instance_lock_file").perform(context)
    lock_handle = open(lock_file, "w", encoding="utf-8")
    try:
        fcntl.flock(lock_handle, fcntl.LOCK_EX | fcntl.LOCK_NB)
    except BlockingIOError as error:
        lock_handle.close()
        raise RuntimeError(
            "Another wheeltec_world simulation is still running. "
            "Close its launch terminal before starting a new one."
        ) from error

    lock_handle.write(f"{os.getpid()}\n")
    lock_handle.flush()
    _instance_lock = lock_handle
    return []


def generate_launch_description():
    use_sim_time = LaunchConfiguration("use_sim_time")
    navigation_enabled = LaunchConfiguration("navigation_enabled")
    gazebo_gui = LaunchConfiguration("gazebo_gui")
    gz_partition = LaunchConfiguration("gz_partition")
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
        # Gazebo spawns the model at its canonical link (base_link), not at
        # base_footprint.  base_link is 0.075 m above base_footprint, so this
        # height places the 0.0375 m-radius wheels exactly on the ground.
        arguments=["-name", "wheeltec_mini_mec", "-topic", "robot_description", "-x", "-2.484", "-y", "1.520", "-z", "0.075"],
    )

    return LaunchDescription([
        DeclareLaunchArgument("use_sim_time", default_value="true"),
        DeclareLaunchArgument(
            "navigation_enabled",
            default_value="false",
            description="Accept Nav2 commands from /cmd_vel_smoothed.",
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
        DeclareLaunchArgument(
            "gz_partition",
            default_value="wheeltec_robot_move_control",
            description="Gazebo Transport partition used to isolate this simulation from unrelated Gazebo instances.",
        ),
        DeclareLaunchArgument(
            "instance_lock_file",
            default_value="/tmp/wheeltec_world.lock",
            description="Process lock preventing multiple wheeltec_world simulations from sharing /clock.",
        ),
        OpaqueFunction(function=_acquire_instance_lock),
        SetEnvironmentVariable("GZ_PARTITION", gz_partition),
        gazebo,
        bridge,
        state_publisher,
        command_mux,
        spawn_robot,
    ])
