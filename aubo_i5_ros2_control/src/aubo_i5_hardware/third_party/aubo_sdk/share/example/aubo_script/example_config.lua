--[[
    功能：设置和获取机械臂配置
]]
local aubo = require('aubo')
local sched = sched or aubo.sched
local math = aubo.math or math

local sleep = sched.sleep
local thread = sched.thread
local sync = sched.sync
local run = sched.run
local kill = sched.kill
function p_example_config()
    setDigitalInputActionDefault()
    setDigitalOutputRunstateDefault()
    setPayload(0, {0,0,0}, {0,0,0}, {0,0,0,0,0,0})
    setTcpOffset({0.0003,0.0001,0.00066,0,0,0})
    Base = {0,0,0,0,0,0}
    Tool = {0,0,0,0,0,0}
    
    function waitForMotionComplete()
        while (getQueueSize() > 0) or (isSteady() == false) do
            sync()
        end
    end
    
    local function calculate_point_to_move_towards(feature, direction, position_distance)
      local posDir={direction[1], direction[2], direction[3]}
      if (math.norm(posDir) < 1e-6) then
          return getTargetTcpPose()
      end
      local direction_vector_normalized=math.normalize(posDir)
      local displacement_pose={direction_vector_normalized[1] * position_distance,direction_vector_normalized[2] * position_distance,direction_vector_normalized[3] * position_distance,0,0,0}

      local wanted_displacement_in_base_frame=poseSub(poseTrans(feature, displacement_pose), feature)
      return poseAdd(getTargetTcpPose(), wanted_displacement_in_base_frame)
    end
    setPlanContext(0, 1, "Program")
    setPlanContext(0, 2, "example_config.lua")
    -- 获取机器人的名字
    robot_name = getName()
    textmsg("The name of robot is "..robot_name)
    -- 获取机器人的自由度(从硬件抽象层读取)
    robot_dof = getDof()
    textmsg("The dof of robot is "..robot_dof)
    -- 获取机器人的伺服控制周期(从硬件抽象层读取)
    cycle_time = getCycletime()
    textmsg("The cycle time of robot is "..cycle_time)
    -- 获取默认的工具端加速度
    default_tool_acc = getDefaultToolAcc()
    textmsg("The default tool acceleraion of robot is "..default_tool_acc)
    -- 获取默认的工具端速度
    default_tool_speed = getDefaultToolSpeed()
    textmsg("The default tool speed of robot is "..default_tool_speed)
    -- 获取默认的关节加速度
    default_joint_acc = getDefaultJointAcc()
    textmsg("The default joint acceleration of robot is "..default_joint_acc)
    -- 获取默认的关节速度
    default_joint_speed = getDefaultJointSpeed()
    textmsg("The default joint speed of robot is "..default_joint_speed)
    -- 获取机器人类型代码
    robot_type = getRobotType()
    textmsg("The type of robot is "..robot_type)
    -- 获取机器人子类型代码
    robot_sub_type = getRobotSubType()
    textmsg("The sub type of robot is "..robot_sub_type)
    -- 获取控制柜类型代码
    control_box_type = getControlBoxType()
    textmsg("The type of control box is "..control_box_type)
    -- 设置和获取安装位姿
    mounting_pose = {0.54897, -0.12120, 0.26253, -3.11, 0.01, 1.57}
    setMountingPose(mounting_pose)
    sleep(1)
    mounting_pose = getMountingPose()
    textmsg("The mounting pose of robot is "..table.concat(mounting_pose, ', '))
    -- 设置和获取碰撞灵敏度等级
    -- 对应AUBOPE set_robot_collision_class(5)
    setCollisionLevel(5)
    sleep(1)
    collision_level = getCollisionLevel()
    textmsg("The collision level of robot is "..collision_level)
    -- 设置和获取碰撞停止类型
    setCollisionStopType(2)
    sleep(1)
    collision_stop_type = getCollisionStopType()
    textmsg("The type of collision stop is "..collision_stop_type)
    -- 设置和获取工具端电压输出范围
    -- 对应AUBOPE set_tool_power_voltage(ToolPowerType.OUT_12V)
    setToolVoltageOutputDomain(24)
    sleep(1)
    tool_voltage_output_domain = getToolVoltageOutputDomain()
    textmsg("The domain of tool voltage output is "..tool_voltage_output_domain)
    -- 获取机器人DH参数
    kinematics_param = getKinematicsParam(true)
    textmsg("The kinematics param of robot is "..table.concat(kinematics_param,', '))
    -- 获取指定温度下的DH参数补偿值
    kinematics_compensate = getKinematicsCompensate(20)
    textmsg("The kinematics compensate of robot is "..table.concat(kinematics_compensate,', '))
    -- 获取可用的末端力矩传感器的名字
    tcp_force_sensor_names = getTcpForceSensorNames()
    textmsg("The names of tcp force sensor are "..table.concat(tcp_force_sensor_names, ', '))
    -- 设置末端力矩传感器
    name = "kw_ftsensor"
    selectTcpForceSensor(name)
    sleep(1)
    -- 是否安装了末端力矩传感器
    has_tcp_force_sensor = hasTcpForceSensor()
    textmsg("Is the sensor installed? "..tostring(has_tcp_force_sensor))
    -- 获取可用的底座力矩传感器的名字
    base_force_sensor_names = getBaseForceSensorNames()
    textmsg("The names of base force sensor are "..table.concat(base_force_sensor_names, ', '))
    -- 获取安全参数校验码 CRC32
    safety_parameters_checksum = getSafetyParametersCheckSum()
    textmsg("The checksum of safety parameters is "..safety_parameters_checksum)
    -- 获取关节最大位置（物理极限）
    joint_max_positions = getJointMaxPositions()
    textmsg("The maximum positions of joint are "..table.concat(joint_max_positions, ', '))
    -- 获取关节最小位置（物理极限）
    joint_min_positions = getJointMinPositions()
    textmsg("The minimum positions of joint are "..table.concat(joint_min_positions, ', '))
    -- 获取关节最大速度（物理极限）
    joint_max_speeds = getJointMaxSpeeds()
    textmsg("The maximum speeds of joint are "..table.concat(joint_max_speeds, ', '))
    -- 获取TCP最大速度（物理极限）
    tcp_max_acc = getTcpMaxAccelerations()
    textmsg("The maximum accelerations of tcp are "..table.concat(tcp_max_acc, ', '))
    -- 设置和获取机器人 gravity
    gravity = {0.6, 0.03, 0.36}
    setGravity(gravity)
    sleep(1)
    gravity = getGravity()
    textmsg("The gravity of robot is "..table.concat(gravity, ', '))
    -- 设置和获取TCP偏移
    -- 对应AUBOPE set_tool_kinematics_param({0, 0, 0.1},{1,0, 0, 0})
    offset = {0, 0, 0.1, 0, 0, 0}
    setTcpOffset(offset)
    sleep(1)
    offset = getTcpOffset()
    textmsg("The offset of tcp is "..table.concat(offset, ', '))
    -- 设置和获取末端负载
    -- cset_tool_dynamics_param(0.5,{0.001,0.002,0.003},{0.0,0.0,0.0,0.0,0.0,0.0})
    setPayload(0.5,{0.001,0.002,0.003},{0,0,0},{0.0,0.0,0.0,0.0,0.0,0.0})
    sleep(1)
    payload = getPayload()
    textmsg("The payload of robot is "..payload)
    -- 末端位姿是否在安全范围之内
    pose = {100, 0.2, 0.45, 0, 0, 0}
    tool_space_in_range = toolSpaceInRange(pose)
    textmsg("Is the tool space in range? "..tostring(tool_space_in_range))
    -- 获取固件升级的进程
    firmware_update_process = getFirmwareUpdateProcess()
    textmsg("The process of firmware update is "..firmware_update_process)
end

local plugin = {
  PRIORITY = 1000, -- set the plugin priority, which determines plugin execution order
  VERSION = "0.1",
  VENDOR = "Aubo Robotics",
}

function plugin:start_robot1()
  print("start_robot1---")
  -- 配置脚本环境
  local env = self.api:loadf(1)
  setmetatable(env, { __index = _G })
  env = require'aubo.scheduler'.trackGlobals(env) or env
  require('compat_env').setfenv(p_example_config, env)
  p_example_config()
end

function plugin:start(api)
  --
  self.api = api
  print("start---")
  self:start_robot1()
end

function plugin:robot_error_handler(name, err)
  --
  print("An error hanppen to robot "..name)
end

-- return our plugin object
return plugin
