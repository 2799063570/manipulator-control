#!/usr/bin/env python3
"""Small retrying lifecycle bringup helper for this teaching Nav2 stack."""

import argparse
import sys
import time

import rclpy
from lifecycle_msgs.msg import State, Transition
from lifecycle_msgs.srv import ChangeState, GetState
from rclpy.executors import ExternalShutdownException
from rclpy.node import Node


ACTIVE_OR_INACTIVE = {
    State.PRIMARY_STATE_ACTIVE,
    State.PRIMARY_STATE_INACTIVE,
}


class LifecycleBringup(Node):
    def __init__(self, args):
        super().__init__("wheeltec_nav2_lifecycle_bringup")
        self.nodes = args.nodes
        self.service_timeout = args.service_timeout
        self.change_timeout = args.change_timeout
        self.retry_delay = args.retry_delay
        self.retries = args.retries
        self.keep_alive = args.keep_alive

        self.change_clients = {
            node_name: self.create_client(ChangeState, self._service_name(node_name, "change_state"))
            for node_name in self.nodes
        }
        self.state_clients = {
            node_name: self.create_client(GetState, self._service_name(node_name, "get_state"))
            for node_name in self.nodes
        }

    @staticmethod
    def _service_name(node_name, service_name):
        return f"{node_name.rstrip('/')}/{service_name}"

    def wait_for_services(self):
        for node_name in self.nodes:
            for client in (self.change_clients[node_name], self.state_clients[node_name]):
                if not client.wait_for_service(timeout_sec=self.service_timeout):
                    self.get_logger().error(
                        f"Lifecycle service {client.srv_name} is not available "
                        f"after {self.service_timeout:.1f}s"
                    )
                    return False
        return True

    def get_state(self, node_name, timeout=None):
        request = GetState.Request()
        future = self.state_clients[node_name].call_async(request)
        rclpy.spin_until_future_complete(self, future, timeout_sec=timeout or self.change_timeout)
        if not future.done() or future.result() is None:
            return None
        return future.result().current_state.id

    def wait_for_state(self, node_name, wanted_state):
        deadline = time.monotonic() + self.change_timeout
        while time.monotonic() < deadline:
            state = self.get_state(node_name, timeout=0.5)
            if state == wanted_state:
                return True
            time.sleep(0.1)
        return self.get_state(node_name, timeout=0.5) == wanted_state

    def transition(self, node_name, transition_id, wanted_state):
        state = self.get_state(node_name)
        if state == wanted_state:
            return True
        if transition_id == Transition.TRANSITION_CONFIGURE and state in ACTIVE_OR_INACTIVE:
            self.get_logger().info(f"{node_name} is already configured")
            return True

        for attempt in range(1, self.retries + 1):
            state = self.get_state(node_name, timeout=0.5)
            if state == wanted_state:
                return True
            if transition_id == Transition.TRANSITION_CONFIGURE and state in ACTIVE_OR_INACTIVE:
                self.get_logger().info(f"{node_name} is already configured")
                return True

            request = ChangeState.Request()
            request.transition.id = transition_id
            future = self.change_clients[node_name].call_async(request)
            rclpy.spin_until_future_complete(self, future, timeout_sec=self.change_timeout)

            if future.done() and future.result() is not None and future.result().success:
                if self.wait_for_state(node_name, wanted_state):
                    return True

            if self.wait_for_state(node_name, wanted_state):
                self.get_logger().warn(
                    f"{node_name} reached state {wanted_state} after a slow lifecycle response"
                )
                return True

            state = self.get_state(node_name, timeout=0.5)
            self.get_logger().warn(
                f"Lifecycle transition {transition_id} for {node_name} did not settle "
                f"(attempt {attempt}/{self.retries}, current state: {state})"
            )
            time.sleep(self.retry_delay)

        return False

    def startup(self):
        if not self.wait_for_services():
            return False

        for node_name in self.nodes:
            self.get_logger().info(f"Configuring {node_name}")
            if not self.transition(node_name, Transition.TRANSITION_CONFIGURE, State.PRIMARY_STATE_INACTIVE):
                self.get_logger().error(f"Failed to configure {node_name}")
                return False

        for node_name in self.nodes:
            self.get_logger().info(f"Activating {node_name}")
            if not self.transition(node_name, Transition.TRANSITION_ACTIVATE, State.PRIMARY_STATE_ACTIVE):
                self.get_logger().error(f"Failed to activate {node_name}")
                return False

        self.get_logger().info("Managed Nav2 nodes are active")
        return True

    def idle(self):
        while rclpy.ok() and self.keep_alive:
            rclpy.spin_once(self, timeout_sec=1.0)


def parse_args(argv):
    parser = argparse.ArgumentParser()
    parser.add_argument("--nodes", nargs="+", required=True)
    parser.add_argument("--service-timeout", type=float, default=15.0)
    parser.add_argument("--change-timeout", type=float, default=8.0)
    parser.add_argument("--retry-delay", type=float, default=1.0)
    parser.add_argument("--retries", type=int, default=3)
    parser.add_argument("--keep-alive", action=argparse.BooleanOptionalAction, default=True)
    args, _ = parser.parse_known_args(argv)
    return args


def main(argv=None):
    rclpy.init(args=None)
    node = LifecycleBringup(parse_args(argv if argv is not None else sys.argv[1:]))

    try:
        if not node.startup():
            return 1
        node.idle()
        return 0
    except (KeyboardInterrupt, ExternalShutdownException):
        return 0
    finally:
        node.destroy_node()
        if rclpy.ok():
            try:
                rclpy.shutdown()
            except RuntimeError:
                pass


if __name__ == "__main__":
    sys.exit(main())
