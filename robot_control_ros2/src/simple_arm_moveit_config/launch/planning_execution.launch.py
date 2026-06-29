from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

from moveit_configs_utils import MoveItConfigsBuilder


def generate_launch_description():
    use_sim_time = LaunchConfiguration("use_sim_time")
    use_rviz = LaunchConfiguration("use_rviz")
    spawn_controllers = LaunchConfiguration("spawn_controllers")

    moveit_config = (
        MoveItConfigsBuilder(
            "simple_2dof_arm",
            package_name="simple_arm_moveit_config"
        )
        .to_moveit_configs()
    )

    # 1. 发布 robot_description，并根据 /joint_states 发布 /tf
    robot_state_publisher = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        name="robot_state_publisher",
        output="screen",
        parameters=[
            moveit_config.robot_description,
            {"use_sim_time": use_sim_time},
        ],
    )

    # 2. 如果你需要 world -> base_link 的固定 TF，可以保留这个节点
    # 如果你的系统已经由别处发布 world -> base_link，可以删掉它，避免重复 TF
    static_tf = Node(
        package="tf2_ros",
        executable="static_transform_publisher",
        name="static_transform_publisher0",
        output="screen",
        arguments=[
            "--x", "0",
            "--y", "0",
            "--z", "0",
            "--roll", "0",
            "--pitch", "0",
            "--yaw", "0",
            "--frame-id", "world",
            "--child-frame-id", "base_link",
        ],
    )

    # 3. MoveIt 核心节点：负责规划、碰撞检测、轨迹生成、轨迹执行管理
    move_group = Node(
        package="moveit_ros_move_group",
        executable="move_group",
        name="move_group",
        output="screen",
        parameters=[
            moveit_config.to_dict(),
            {"use_sim_time": use_sim_time},
        ],
    )

    # 4. RViz，可选启动
    rviz_config = str(moveit_config.package_path / "config" / "moveit.rviz")

    rviz = Node(
        package="rviz2",
        executable="rviz2",
        name="rviz2",
        output="screen",
        arguments=["-d", rviz_config],
        parameters=[
            moveit_config.robot_description,
            moveit_config.robot_description_semantic,
            moveit_config.robot_description_kinematics,
            moveit_config.planning_pipelines,
            {"use_sim_time": use_sim_time},
        ],
        condition=IfCondition(use_rviz),
    )

    # 5. 控制器 spawner
    # 注意：这里假设 /controller_manager 已经由 Gazebo 插件或真实驱动启动
    joint_state_broadcaster_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=[
            "joint_state_broadcaster",
            "--controller-manager",
            "/controller_manager",
        ],
        output="screen",
        condition=IfCondition(spawn_controllers),
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
        condition=IfCondition(spawn_controllers),
    )

    return LaunchDescription([
        DeclareLaunchArgument(
            "use_sim_time",
            default_value="false",
            description="Use simulation clock. Set true for Gazebo.",
        ),
        DeclareLaunchArgument(
            "use_rviz",
            default_value="true",
            description="Start RViz with MoveIt MotionPlanning plugin.",
        ),
        DeclareLaunchArgument(
            "spawn_controllers",
            default_value="true",
            description="Spawn joint_state_broadcaster and arm_controller.",
        ),

        robot_state_publisher,
        static_tf,
        move_group,
        rviz,

        joint_state_broadcaster_spawner,
        arm_controller_spawner,
    ])