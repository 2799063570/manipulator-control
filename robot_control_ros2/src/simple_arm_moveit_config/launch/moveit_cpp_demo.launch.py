from launch import LaunchDescription
from launch_ros.actions import Node

from moveit_configs_utils import MoveItConfigsBuilder


def generate_launch_description():
    moveit_config = (
        MoveItConfigsBuilder(
            "simple_2dof_arm",
            package_name="simple_arm_moveit_config",
        )
        .planning_pipelines(default_planning_pipeline="ompl", pipelines=["ompl"])
        .to_moveit_configs()
    )

    moveit_cpp_demo = Node(
        package="simple_arm_moveit_config",
        executable="moveit_cpp_demo",
        output="screen",
        parameters=[moveit_config.to_dict()],
    )

    return LaunchDescription([moveit_cpp_demo])
