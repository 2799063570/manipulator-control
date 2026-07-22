"""Choose manual or Nav2 velocity commands with a safe timeout."""

import time

from geometry_msgs.msg import Twist
import rclpy
from rclpy.node import Node


class CmdMux(Node):
    """Forward the freshest allowed velocity command to the robot base.

    Manual teleoperation always takes precedence. Navigation commands are
    ignored until the ``navigation_enabled`` parameter is true.
    """

    def __init__(self):
        super().__init__("wheeltec_cmd_mux")
        self.declare_parameter("navigation_enabled", False)
        self.declare_parameter("navigation_topic", "/cmd_vel_smoothed")
        self.declare_parameter("teleop_timeout", 0.5)
        self.declare_parameter("navigation_timeout", 0.5)
        self.declare_parameter("publish_rate", 20.0)

        self._teleop_command = None
        self._teleop_stamp = 0.0
        self._navigation_command = None
        self._navigation_stamp = 0.0
        self._active_source = None

        self.create_subscription(Twist, "/cmd_vel_teleop", self._teleop_callback, 10)
        navigation_topic = str(self.get_parameter("navigation_topic").value)
        self.create_subscription(Twist, navigation_topic, self._navigation_callback, 10)
        self._publisher = self.create_publisher(Twist, "/cmd_vel", 10)

        rate = float(self.get_parameter("publish_rate").value)
        if rate <= 0.0:
            raise ValueError("publish_rate must be greater than zero")
        self.create_timer(1.0 / rate, self._publish_command)

    def _teleop_callback(self, command):
        self._teleop_command = command
        self._teleop_stamp = time.monotonic()

    def _navigation_callback(self, command):
        self._navigation_command = command
        self._navigation_stamp = time.monotonic()

    def _is_fresh(self, stamp, timeout):
        return time.monotonic() - stamp <= float(timeout)

    def _publish_command(self):
        teleop_fresh = self._teleop_command is not None and self._is_fresh(
            self._teleop_stamp, self.get_parameter("teleop_timeout").value
        )
        navigation_fresh = self._navigation_command is not None and self._is_fresh(
            self._navigation_stamp, self.get_parameter("navigation_timeout").value
        )
        navigation_enabled = bool(self.get_parameter("navigation_enabled").value)

        if teleop_fresh:
            command, source = self._teleop_command, "teleop"
        elif navigation_enabled and navigation_fresh:
            command, source = self._navigation_command, "navigation"
        else:
            command, source = Twist(), None

        if source != self._active_source:
            self.get_logger().info("Active velocity source: %s" % (source or "stopped"))
            self._active_source = source
        self._publisher.publish(command)


def main():
    rclpy.init()
    node = CmdMux()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()
