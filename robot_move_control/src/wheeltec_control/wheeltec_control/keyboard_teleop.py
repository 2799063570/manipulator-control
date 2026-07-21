"""A small terminal keyboard teleoperation node for mapping sessions."""

import select
import sys
import termios
import tty

from geometry_msgs.msg import Twist
import rclpy
from rclpy.node import Node


HELP = """
Keyboard control (publish to /cmd_vel_teleop)
  w/s: forward/backward      a/d: left/right
  q/e: rotate left/right     space: stop
  Ctrl-C: quit
""".strip()


class KeyboardTeleop(Node):
    def __init__(self):
        super().__init__("wheeltec_keyboard_teleop")
        self.declare_parameter("linear_speed", 0.15)
        self.declare_parameter("lateral_speed", 0.15)
        self.declare_parameter("angular_speed", 0.5)
        self._publisher = self.create_publisher(Twist, "/cmd_vel_teleop", 10)

    def publish_key(self, key):
        command = Twist()
        linear_speed = float(self.get_parameter("linear_speed").value)
        lateral_speed = float(self.get_parameter("lateral_speed").value)
        angular_speed = float(self.get_parameter("angular_speed").value)
        bindings = {
            "w": (linear_speed, 0.0, 0.0),
            "s": (-linear_speed, 0.0, 0.0),
            "a": (0.0, lateral_speed, 0.0),
            "d": (0.0, -lateral_speed, 0.0),
            "q": (0.0, 0.0, angular_speed),
            "e": (0.0, 0.0, -angular_speed),
            " ": (0.0, 0.0, 0.0),
        }
        if key not in bindings:
            return
        command.linear.x, command.linear.y, command.angular.z = bindings[key]
        self._publisher.publish(command)

    def stop(self):
        self._publisher.publish(Twist())


def _read_key():
    ready, _, _ = select.select([sys.stdin], [], [], 0.1)
    return sys.stdin.read(1) if ready else ""


def main():
    if not sys.stdin.isatty():
        raise RuntimeError("keyboard_teleop must be run directly from an interactive terminal")

    rclpy.init()
    node = KeyboardTeleop()
    original_settings = termios.tcgetattr(sys.stdin)
    print(HELP)
    try:
        tty.setraw(sys.stdin.fileno())
        while rclpy.ok():
            key = _read_key()
            if key == "\x03":
                break
            node.publish_key(key)
    finally:
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, original_settings)
        node.stop()
        node.destroy_node()
        rclpy.shutdown()
