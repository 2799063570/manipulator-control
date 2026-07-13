from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, TimerAction
from launch.conditions import IfCondition
from launch.substitutions import Command, FindExecutable, LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    robot_ip = LaunchConfiguration("robot_ip")
    enable_motion = LaunchConfiguration("enable_motion")
    moveit_share = FindPackageShare("aubo_i5_moveit_config")
    hardware_share = FindPackageShare("aubo_i5_hardware")
    xacro_file = PathJoinSubstitution([moveit_share, "config", "aubo_i5.urdf.xacro"])
    initial_positions = PathJoinSubstitution([moveit_share, "config", "initial_positions.yaml"])
    controllers = PathJoinSubstitution([hardware_share, "config", "real_controllers.yaml"])
    robot_description = {
        "robot_description": ParameterValue(
            Command([
                FindExecutable(name="xacro"), " ", xacro_file, " ",
                "hardware_type:=real ",
                "robot_ip:=", robot_ip, " ",
                "enable_motion:=", enable_motion, " ",
                "initial_positions_file:=", initial_positions, " ",
                "ros2_controllers_file:=", controllers,
            ]),
            value_type=str,
        )
    }

    control_node = Node(
        package="controller_manager",
        executable="ros2_control_node",
        parameters=[robot_description, controllers, {"use_sim_time": False}],
        output="screen",
    )
    state_publisher = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        parameters=[robot_description, {"use_sim_time": False}],
        output="screen",
    )
    joint_state_broadcaster = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["joint_state_broadcaster", "--controller-manager", "/controller_manager"],
        output="screen",
    )
    # Deliberately opt-in: in read-only mode no FollowJointTrajectory action exists.
    arm_controller = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["arm_trajectory_controller", "--controller-manager", "/controller_manager"],
        condition=IfCondition(enable_motion),
        output="screen",
    )

    return LaunchDescription([
        DeclareLaunchArgument(
            "robot_ip", default_value="",
            description="Required AUBO controller IP; no default real address is stored."),
        DeclareLaunchArgument(
            "enable_motion", default_value="false",
            description="Explicitly permit Servo writes and the trajectory controller."),
        control_node,
        state_publisher,
        TimerAction(period=2.0, actions=[joint_state_broadcaster]),
        TimerAction(period=4.0, actions=[arm_controller]),
    ])
