from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import Command, FindExecutable, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
from launch_ros.substitutions import FindPackageShare

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
    moveit_config = (
        MoveItConfigsBuilder("aubo_i5", package_name="aubo_i5_moveit_config")
        .planning_pipelines(default_planning_pipeline="ompl", pipelines=["ompl"])
        .to_moveit_configs()
    )
    rviz_config = moveit_config.package_path / "config" / "moveit.rviz"
    moveit_config_share = FindPackageShare("aubo_i5_moveit_config")

    robot_description_xacro = PathJoinSubstitution(
        [moveit_config_share, "config", "aubo_i5.urdf.xacro"]
    )
    initial_positions_file = PathJoinSubstitution(
        [moveit_config_share, "config", "initial_positions.yaml"]
    )
    ros2_controllers_file = PathJoinSubstitution(
        [moveit_config_share, "config", "ros2_controllers.yaml"]
    )
    gazebo_robot_description = {
        "robot_description": ParameterValue(
            Command(
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
            ),
            value_type=str,
        )
    }

    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution(
                [
                    FindPackageShare("aubo_i5_gazebo"),
                    "launch",
                    "aubo_i5_gazebo.launch.py",
                ]
            )
        )
    )

    move_group = Node(
        package="moveit_ros_move_group",
        executable="move_group",
        output="screen",
        parameters=[
            moveit_config.to_dict(),
            gazebo_robot_description,
            OMPL_PARAMETERS,
            {"use_sim_time": True},
        ],
    )

    rviz = Node(
        package="rviz2",
        executable="rviz2",
        output="screen",
        arguments=["-d", str(rviz_config)],
        parameters=[
            moveit_config.to_dict(),
            gazebo_robot_description,
            OMPL_PARAMETERS,
            {"use_sim_time": True},
        ],
    )

    return LaunchDescription([gazebo, move_group, rviz])
