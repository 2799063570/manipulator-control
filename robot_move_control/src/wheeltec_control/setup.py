from setuptools import find_packages, setup


package_name = "wheeltec_control"

setup(
    name=package_name,
    version="0.1.0",
    packages=find_packages(exclude=["test"]),
    data_files=[
        ("share/ament_index/resource_index/packages", ["resource/" + package_name]),
        ("share/" + package_name, ["package.xml"]),
    ],
    install_requires=["setuptools"],
    zip_safe=True,
    maintainer="Manipulator-control maintainers",
    maintainer_email="todo@example.com",
    description="Manual teleoperation and safe command selection for Wheeltec.",
    license="Apache-2.0",
    entry_points={
        "console_scripts": [
            "cmd_mux = wheeltec_control.cmd_mux:main",
            "keyboard_teleop = wheeltec_control.keyboard_teleop:main",
            "map_loader = wheeltec_control.map_loader:main",
        ],
    },
)
