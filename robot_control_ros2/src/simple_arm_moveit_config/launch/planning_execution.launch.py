from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

from moveit_configs_utils import MoveItConfigsBuilder


OMPL_PARAMETERS = {
    "default_planning_pipeline": "ompl",
    "planning_pipelines": ["ompl"],
    "ompl.planning_plugins": ["ompl_interface/OMPLPlanner"],
    "ompl.request_adapters": [
        "default_planning_request_adapters/ResolveConstraintFrames",
        "default_planning_request_adapters/ValidateWorkspaceBounds",
        "default_planning_request_adapters/CheckStartStateBounds",
        "default_planning_request_adapters/CheckStartStateCollision",
    ],
    "ompl.response_adapters": [
        "default_planning_response_adapters/AddTimeOptimalParameterization",
        "default_planning_response_adapters/ValidateSolution",
        "default_planning_response_adapters/DisplayMotionPath",
    ],
    "ompl.start_state_max_bounds_error": 0.1,
    "ompl.planner_configs.RRTConnectkConfigDefault.type": "geometric::RRTConnect",
    "ompl.planner_configs.RRTConnectkConfigDefault.range": 0.0,
    "ompl.planner_configs.RRTkConfigDefault.type": "geometric::RRT",
    "ompl.planner_configs.RRTkConfigDefault.range": 0.0,
    "ompl.planner_configs.RRTkConfigDefault.goal_bias": 0.05,
    "ompl.planner_configs.RRTstarkConfigDefault.type": "geometric::RRTstar",
    "ompl.planner_configs.RRTstarkConfigDefault.range": 0.0,
    "ompl.planner_configs.RRTstarkConfigDefault.goal_bias": 0.05,
    "ompl.planner_configs.RRTstarkConfigDefault.delay_collision_checking": 1,
    "ompl.planner_configs.PRMkConfigDefault.type": "geometric::PRM",
    "ompl.planner_configs.PRMkConfigDefault.max_nearest_neighbors": 10,
    "ompl.planner_configs.PRMstarkConfigDefault.type": "geometric::PRMstar",
    "ompl.planner_configs.ESTkConfigDefault.type": "geometric::EST",
    "ompl.planner_configs.ESTkConfigDefault.range": 0.0,
    "ompl.planner_configs.ESTkConfigDefault.goal_bias": 0.05,
    "ompl.planner_configs.KPIECEkConfigDefault.type": "geometric::KPIECE",
    "ompl.planner_configs.KPIECEkConfigDefault.range": 0.0,
    "ompl.planner_configs.KPIECEkConfigDefault.goal_bias": 0.05,
    "ompl.planner_configs.KPIECEkConfigDefault.border_fraction": 0.9,
    "ompl.planner_configs.KPIECEkConfigDefault.failed_expansion_score_factor": 0.5,
    "ompl.planner_configs.KPIECEkConfigDefault.min_valid_path_fraction": 0.5,
    "ompl.planner_configs.BKPIECEkConfigDefault.type": "geometric::BKPIECE",
    "ompl.planner_configs.BKPIECEkConfigDefault.range": 0.0,
    "ompl.planner_configs.BKPIECEkConfigDefault.border_fraction": 0.9,
    "ompl.planner_configs.BKPIECEkConfigDefault.failed_expansion_score_factor": 0.5,
    "ompl.planner_configs.BKPIECEkConfigDefault.min_valid_path_fraction": 0.5,
    "ompl.planner_configs.LBKPIECEkConfigDefault.type": "geometric::LBKPIECE",
    "ompl.planner_configs.LBKPIECEkConfigDefault.range": 0.0,
    "ompl.planner_configs.LBKPIECEkConfigDefault.border_fraction": 0.9,
    "ompl.planner_configs.LBKPIECEkConfigDefault.min_valid_path_fraction": 0.5,
    "ompl.arm.default_planner_config": "RRTConnectkConfigDefault",
    "ompl.arm.planner_configs": [
        "RRTConnectkConfigDefault",
        "RRTkConfigDefault",
        "RRTstarkConfigDefault",
        "PRMkConfigDefault",
        "PRMstarkConfigDefault",
        "ESTkConfigDefault",
        "KPIECEkConfigDefault",
        "BKPIECEkConfigDefault",
        "LBKPIECEkConfigDefault",
    ],
}


def generate_launch_description():
    use_sim_time = LaunchConfiguration("use_sim_time")
    use_rviz = LaunchConfiguration("use_rviz")
    use_ros2_control = LaunchConfiguration("use_ros2_control")
    spawn_controllers = LaunchConfiguration("spawn_controllers")

    moveit_config = (
        MoveItConfigsBuilder(
            "simple_2dof_arm",
            package_name="simple_arm_moveit_config",
        )
        .planning_pipelines(default_planning_pipeline="ompl", pipelines=["ompl"])
        .to_moveit_configs()
    )

    ros2_controllers_path = (
        moveit_config.package_path / "config" / "ros2_controllers.yaml"
    )
    rviz_config = moveit_config.package_path / "config" / "moveit.rviz"

    # Publish robot_description and derive TF from /joint_states.
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

    # Publish the fixed world -> base_link transform used by this MoveIt config.
    static_tf = Node(
        package="tf2_ros",
        executable="static_transform_publisher",
        name="static_transform_publisher0",
        output="screen",
        arguments=[
            "--x",
            "0",
            "--y",
            "0",
            "--z",
            "0",
            "--roll",
            "0",
            "--pitch",
            "0",
            "--yaw",
            "0",
            "--frame-id",
            "world",
            "--child-frame-id",
            "base_link",
        ],
    )

    # Start ros2_control's controller manager with the fake hardware system
    # described in simple_2dof_arm.ros2_control.xacro.
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

    # Start MoveIt's planning and execution server.
    move_group = Node(
        package="moveit_ros_move_group",
        executable="move_group",
        output="screen",
        parameters=[
            moveit_config.to_dict(),
            OMPL_PARAMETERS,
            {"use_sim_time": use_sim_time},
        ],
    )

    # Optional RViz session with the MotionPlanning plugin configured.
    rviz = Node(
        package="rviz2",
        executable="rviz2",
        output="screen",
        arguments=["-d", str(rviz_config)],
        parameters=[
            moveit_config.to_dict(),
            OMPL_PARAMETERS,
            {"use_sim_time": use_sim_time},
        ],
        condition=IfCondition(use_rviz),
    )

    # Load and activate controllers managed by /controller_manager.
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

    return LaunchDescription(
        [
            DeclareLaunchArgument(
                "use_sim_time",
                default_value="false",
                description="Use simulation clock. Set true when another simulator publishes /clock.",
            ),
            DeclareLaunchArgument(
                "use_rviz",
                default_value="true",
                description="Start RViz with the MoveIt MotionPlanning plugin.",
            ),
            DeclareLaunchArgument(
                "use_ros2_control",
                default_value="true",
                description="Start ros2_control_node for the fake hardware system.",
            ),
            DeclareLaunchArgument(
                "spawn_controllers",
                default_value="true",
                description="Spawn joint_state_broadcaster and arm_controller.",
            ),
            robot_state_publisher,
            static_tf,
            ros2_control_node,
            move_group,
            rviz,
            joint_state_broadcaster_spawner,
            arm_controller_spawner,
        ]
    )
