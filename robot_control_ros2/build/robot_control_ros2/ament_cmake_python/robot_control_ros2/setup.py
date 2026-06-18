from setuptools import find_packages
from setuptools import setup

setup(
    name='robot_control_ros2',
    version='0.0.0',
    packages=find_packages(
        include=('robot_control_ros2', 'robot_control_ros2.*')),
)
