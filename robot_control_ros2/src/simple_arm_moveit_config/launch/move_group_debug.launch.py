from pprint import pformat

from launch import LaunchDescription
from launch.actions import OpaqueFunction
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


def build_moveit_config():
    return (
        MoveItConfigsBuilder("simple_2dof_arm", package_name="simple_arm_moveit_config")
        .planning_pipelines(default_planning_pipeline="ompl", pipelines=["ompl"])
        .to_moveit_configs()
    )


def print_debug_params(context):
    moveit_config = (
        build_moveit_config()
    )
    params = moveit_config.to_dict()

    print("\n=== move_group_debug parameters ===")
    print("default_planning_pipeline:")
    print(pformat(params.get("default_planning_pipeline")))
    print("planning_pipelines:")
    print(pformat(params.get("planning_pipelines")))
    print("ompl:")
    print(pformat(params.get("ompl")))
    print("flat OMPL override parameters:")
    print(pformat(OMPL_PARAMETERS))
    print("=== end move_group_debug parameters ===\n")

    return []


def generate_launch_description():
    moveit_config = build_moveit_config()

    move_group = Node(
        package="moveit_ros_move_group",
        executable="move_group",
        name="move_group",
        output="screen",
        emulate_tty=True,
        parameters=[moveit_config.to_dict(), OMPL_PARAMETERS],
    )

    return LaunchDescription([OpaqueFunction(function=print_debug_params), move_group])
