from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess, DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

def spawn(node_name, x=2.0, y=2.0, theta=0.0, turtle_name='turtle2'):
    return Node(package='turtlesim_exercise', executable='spawn_turtle',
            name=node_name, 
            parameters=[{"x":x, "y":y, "theta":theta, "turtle_name":turtle_name}])

def tf_broadcaster(node_name, turtle_name='turtle1'):
    return Node(package='turtlesim_exercise', executable='turtle_tf_broadcaster',
            name=node_name,
            parameters=[{"turtle_name": turtle_name}])

def escort(node_name, x="0.0", y="0.0", frame_id="turtle1", child_frame_id="escort1"):
    return Node(package='tf2_ros', executable="static_transform_publisher",
                name=node_name,
                arguments=["--x",x,"--y",y,"--frame-id",frame_id,"--child-frame-id",child_frame_id]
            )
def tf_listener(node_name,target_frame,source_frame):
  return Node(package="turtlesim_exercise",executable="turtle_listener_control",
              name=node_name,
              parameters=[{"target_frame":target_frame,"source_frame":source_frame}]
            )

def generate_launch_description():

    # 启动 turtlesim_node 节点
    turtlesim_node = Node(package="turtlesim", executable="turtlesim_node", name="t1")
    # 生成一只新乌龟
    spawn1 = spawn(node_name="spawn1")
    spawn2 = spawn(node_name="spawn2",x=2.0,y=7.0,turtle_name="turtle3")
    spawn3 = spawn(node_name="spawn3",x=7.0,y=2.0,theta=1.57,turtle_name="turtle4")
    # tf 广播
    tf_broadcaster1 = tf_broadcaster(node_name="tf_broadcaster1")
    tf_broadcaster2 = tf_broadcaster(node_name="tf_broadcaster2",turtle_name="turtle2")
    tf_broadcaster3 = tf_broadcaster(node_name="tf_broadcaster3",turtle_name="turtle3")
    tf_broadcaster4 = tf_broadcaster(node_name="tf_broadcaster4",turtle_name="turtle4")

    escort1 = escort(node_name="static_transform_publisher1",x="-1")
    escort2 = escort(node_name="static_transform_publisher2",y="1",child_frame_id="escort2")
    escort3 = escort(node_name="static_transform_publisher3",y="-1",child_frame_id="escort3")

    # tf 监听
    tf_listener1 = tf_listener(node_name="tf_listener1",target_frame="turtle2",source_frame="escort1")
    tf_listener2 = tf_listener(node_name="tf_listener2",target_frame="turtle3",source_frame="escort2")
    tf_listener3 = tf_listener(node_name="tf_listener3",target_frame="turtle4",source_frame="escort3")

    return LaunchDescription([turtlesim_node,spawn1,spawn2,spawn3,
            tf_broadcaster1,tf_broadcaster2,tf_broadcaster3,tf_broadcaster4,
            escort1,escort2,escort3,
            tf_listener1,tf_listener2,tf_listener3
    ])