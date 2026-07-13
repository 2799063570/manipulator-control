from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, TimerAction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.parameter_descriptions import ParameterValue
from launch.substitutions import Command, FindExecutable, LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

# 需要注意的是 虚拟机显卡渲染有问题 我们需要设置软件渲染 即LIBGL_ALWAYS_SOFTWARE=1
def generate_launch_description():
    use_sim_time = LaunchConfiguration("use_sim_time")
    world = LaunchConfiguration("world")

    gazebo_share = FindPackageShare("simple_arm_gazebo")
    moveit_config_share = FindPackageShare("simple_arm_moveit_config")

    robot_description_xacro = PathJoinSubstitution(
        [moveit_config_share, "config", "simple_2dof_arm.urdf.xacro"]
    )
    initial_positions_file = PathJoinSubstitution(
        [moveit_config_share, "config", "initial_positions.yaml"]
    )
    ros2_controllers_file = PathJoinSubstitution(
        [moveit_config_share, "config", "ros2_controllers.yaml"]
    )

    # 构造命令行指令 强制输出URDF XML
    robot_description_content = Command(
        [
            FindExecutable(name="xacro"),
            " ",
            robot_description_xacro,
            " ",
            "hardware_type:=gazebo",
            " ",
            "use_world_joint:=true",
            " ",
            "initial_positions_file:=",
            initial_positions_file,
            " ",
            "ros2_controllers_file:=",
            ros2_controllers_file,
        ]
    )
    robot_description = {
        "robot_description": ParameterValue(robot_description_content, value_type=str)
    }

    gz_sim = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([FindPackageShare("ros_gz_sim"), "launch", "gz_sim.launch.py"])
        ),
        launch_arguments={"gz_args": ["-r -v 4 ", world]}.items(),
    )

    clock_bridge = Node(
        package="ros_gz_bridge",
        executable="parameter_bridge",
        output="screen",
        arguments=[
            "/world/empty/clock@rosgraph_msgs/msg/Clock[gz.msgs.Clock",
        ],
        remappings=[
            ("/world/empty/clock", "/clock"),
        ],
    )

    robot_state_publisher = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        output="screen",
        parameters=[robot_description, {"use_sim_time": use_sim_time}],
    )

    spawn_robot = Node(
        package="ros_gz_sim",
        executable="create",
        output="screen",
        arguments=[
            "-name",
            "simple_2dof_arm",
            "-topic",
            "robot_description",
            "-x",
            "0",
            "-y",
            "0",
            "-z",
            "0",
            "-allow_renaming",
            "true",
        ],
    )

    joint_state_broadcaster_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=[
            "joint_state_broadcaster",
            "--controller-manager",
            "/controller_manager",
        ],
        output="screen",
    )

    arm_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=[
            "arm_controller",
            "--controller-manager",
            "/controller_manager",
        ],
        output="screen",
    )

    delayed_controller_spawners = TimerAction(
        period=5.0,
        actions=[joint_state_broadcaster_spawner, arm_controller_spawner],
    )

    return LaunchDescription(
        [
            DeclareLaunchArgument(
                "use_sim_time",
                default_value="true",
                description="Use Gazebo simulation clock.",
            ),
            DeclareLaunchArgument(
                "world",
                default_value=PathJoinSubstitution(
                    [gazebo_share, "worlds", "empty.sdf"]
                ),
                description="Gazebo world file.",
            ),
            gz_sim,
            clock_bridge,
            robot_state_publisher,
            spawn_robot,
            delayed_controller_spawners,
        ]
    )
