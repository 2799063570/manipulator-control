from pprint import pformat

from launch import LaunchDescription
from launch.actions import OpaqueFunction

from moveit_configs_utils import MoveItConfigsBuilder


def print_config(context):
    moveit_config = (
        MoveItConfigsBuilder("simple_2dof_arm", package_name="simple_arm_moveit_config")
        .planning_pipelines(default_planning_pipeline="ompl", pipelines=["ompl"])
        .to_moveit_configs()
    )
    params = moveit_config.to_dict()

    print("\n=== MoveIt planning pipeline parameters ===")
    print("default_planning_pipeline:")
    print(pformat(params.get("default_planning_pipeline")))
    print("planning_pipelines:")
    print(pformat(params.get("planning_pipelines")))
    print("ompl:")
    print(pformat(params.get("ompl")))
    ompl = params.get("ompl", {})
    print("ompl.planning_plugins type/value:")
    print(type(ompl.get("planning_plugins")).__name__, pformat(ompl.get("planning_plugins")))
    print("ompl.request_adapters type/value:")
    print(type(ompl.get("request_adapters")).__name__, pformat(ompl.get("request_adapters")))
    print("ompl.response_adapters type/value:")
    print(type(ompl.get("response_adapters")).__name__, pformat(ompl.get("response_adapters")))
    print("robot_description_kinematics:")
    print(pformat(params.get("robot_description_kinematics")))
    print("=== End MoveIt planning pipeline parameters ===\n")

    return []


def generate_launch_description():
    return LaunchDescription([OpaqueFunction(function=print_config)])
