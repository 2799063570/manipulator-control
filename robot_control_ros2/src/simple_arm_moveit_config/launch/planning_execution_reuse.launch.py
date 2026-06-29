from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.conditions import IfCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.actions import SetParameter
from launch_ros.substitutions import FindPackageShare

from moveit_configs_utils import MoveItConfigsBuilder


def include_moveit_launch(filename, condition=None):
    return IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution(
                [
                    FindPackageShare("simple_arm_moveit_config"),
                    "launch",
                    filename,
                ]
            )
        ),
        condition=condition,
    )


def generate_launch_description():
    use_sim_time = LaunchConfiguration("use_sim_time")
    use_ros2_control = LaunchConfiguration("use_ros2_control")
    use_rviz = LaunchConfiguration("use_rviz")
    spawn_controllers = LaunchConfiguration("spawn_controllers")

    moveit_config = (
        MoveItConfigsBuilder(
            "simple_2dof_arm",
            package_name="simple_arm_moveit_config",
        )
        .to_moveit_configs()
    )

    ros2_controllers_path = (
        moveit_config.package_path / "config" / "ros2_controllers.yaml"
    )

    # The generated package has launch files for RSP, fixed TFs, MoveIt, RViz,
    # and controller spawners. It does not include a standalone launch file for
    # ros2_control_node, so this learning launch starts that node explicitly.
    ros2_control_node = Node(
        package="controller_manager",
        executable="ros2_control_node",
        parameters=[
            moveit_config.robot_description,
            str(ros2_controllers_path),
            {"use_sim_time": use_sim_time},
        ],
        output="screen",
        condition=IfCondition(use_ros2_control),
    )

    return LaunchDescription(
        [
            DeclareLaunchArgument(
                "use_sim_time",
                default_value="false",
                description="Use simulation clock. Set true when another simulator publishes /clock.",
            ),
            DeclareLaunchArgument(
                "use_ros2_control",
                default_value="true",
                description="Start ros2_control_node for the fake hardware system.",
            ),
            DeclareLaunchArgument(
                "use_rviz",
                default_value="true",
                description="Include moveit_rviz.launch.py.",
            ),
            DeclareLaunchArgument(
                "spawn_controllers",
                default_value="true",
                description="Include spawn_controllers.launch.py.",
            ),
            SetParameter(name="use_sim_time", value=use_sim_time),
            include_moveit_launch("rsp.launch.py"),
            include_moveit_launch("static_virtual_joint_tfs.launch.py"),
            ros2_control_node,
            include_moveit_launch("move_group.launch.py"),
            include_moveit_launch(
                "moveit_rviz.launch.py",
                condition=IfCondition(use_rviz),
            ),
            include_moveit_launch(
                "spawn_controllers.launch.py",
                condition=IfCondition(spawn_controllers),
            ),
        ]
    )
