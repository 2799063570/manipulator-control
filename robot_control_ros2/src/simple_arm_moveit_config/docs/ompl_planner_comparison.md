# OMPL Planner Comparison

This package includes a small benchmark node for comparing OMPL planners on the same
`arm` joint-space target.

## 1. Inspect the OMPL config

```bash
cat install/simple_arm_moveit_config/share/simple_arm_moveit_config/config/ompl_planning.yaml
```

The configured planners are:

- `RRTConnectkConfigDefault`
- `RRTkConfigDefault`
- `RRTstarkConfigDefault`
- `PRMkConfigDefault`
- `PRMstarkConfigDefault`
- `ESTkConfigDefault`
- `KPIECEkConfigDefault`
- `BKPIECEkConfigDefault`
- `LBKPIECEkConfigDefault`

## 2. Start MoveIt

In terminal 1:

```bash
cd ~/cpp_practice/robot_control_ros2
source install/setup.bash
ros2 launch simple_arm_moveit_config demo.launch.py
```

Wait until RViz says the planning group is ready.

## 3. Run the comparison

In terminal 2:

```bash
cd ~/cpp_practice/robot_control_ros2
source install/setup.bash
ros2 launch simple_arm_moveit_config planner_comparison.launch.py
```

The default target is:

- `joint1 = 1.0`
- `joint2 = 0.6`

Each planner runs 5 times against that same target.

## 4. Read the results

The node writes:

- `planner_comparison.csv`: per-run raw data
- `planner_comparison.md`: summary table

The summary table reports:

- success count and success rate
- average planning wall time
- average timed trajectory duration
- average joint-space path length
- average number of trajectory points

Use these values to compare speed, success stability, and trajectory shape.
