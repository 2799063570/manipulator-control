"""Load a saved map once Nav2's map_server lifecycle node becomes available."""

from nav2_msgs.srv import LoadMap
import rclpy
from rclpy.node import Node


class MapLoader(Node):
    """Wait for map_server and load the path supplied by the navigation launch."""

    def __init__(self):
        super().__init__("wheeltec_map_loader")
        self.declare_parameter("map_file", "")
        self._map_file = str(self.get_parameter("map_file").value)
        if not self._map_file:
            raise RuntimeError("map_file must be an absolute path to a map YAML file")

        self._client = self.create_client(LoadMap, "/map_server/load_map")
        self._request_sent = False
        self.create_timer(0.5, self._try_load)

    def _try_load(self):
        if self._request_sent or not self._client.service_is_ready():
            return
        request = LoadMap.Request()
        request.map_url = self._map_file
        self._request_sent = True
        future = self._client.call_async(request)
        future.add_done_callback(self._handle_result)

    def _handle_result(self, future):
        try:
            response = future.result()
        except Exception as exc:  # Service failures are retried after map_server recovers.
            self.get_logger().warn("Map load request failed: %s" % exc)
            self._request_sent = False
            return

        if response.result == LoadMap.Response.RESULT_SUCCESS:
            self.get_logger().info("Loaded map: %s" % self._map_file)
            return

        self.get_logger().error("map_server rejected map '%s' (result %d)" % (self._map_file, response.result))


def main():
    rclpy.init()
    node = MapLoader()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()
