--[[
    功能：获取机械臂相关的状态
]]
local aubo = require('aubo')
local sched = sched or aubo.sched
local math = aubo.math or math

local sleep = sched.sleep
local thread = sched.thread
local sync = sched.sync
local run = sched.run
local kill = sched.kill
function p_example_robot_state()
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
    setPlanContext(0, 2, "robot_state.lua")
    -- 获取机器人的模式状态
    mode_type = getRobotModeType()
    textmsg("The type of robot mode is "..mode_type)
    -- 获取安全模式
    safety_mode_type = getSafetyModeType()
    textmsg("The type of safety mode is "..safety_mode_type)
    -- 机器人是否已经停止下来
    is_steady = isSteady()
    textmsg("Is the robot steady? "..tostring(is_steady))
    -- 机器人是否已经在安全限制之内
    is_within_safety_limits = isWithinSafetyLimits()
    textmsg("Is the robot within safety limits? "..tostring(is_within_safety_limits))
    -- 获取TCP的位姿
    -- 对应AUBOPE cp_pose =get_current_waypoint
    tcp_pose = getTcpPose()
    textmsg("The pose of tcp is "..table.concat(tcp_pose, ', '))
    -- 获取当前目标位姿
    target_tcp_pose = getTargetTcpPose()
    textmsg("The pose of target tcp is "..table.concat(target_tcp_pose, ', '))
    -- 获取工具端的位姿（不带TCP偏移）
    tool_pose = getToolPose()
    textmsg("The pose of tool is "..table.concat(tool_pose, ', '))
    -- 获取TCP速度
    tcp_speed = getTcpSpeed()
    textmsg("The speed of tcp is "..table.concat(tcp_speed, ', '))
    -- 获取TCP的力/力矩
    tcp_force = getTcpForce()
    textmsg("The force of tcp is "..table.concat(tcp_force, ', '))
    -- 获取肘部的位置
    elbow_position = getElbowPosistion()
    textmsg("The position of elbow is "..table.concat(elbow_position, ', '))
    -- 获取肘部速度
    elbow_velocity = getElbowVelocity()
    textmsg("The velocity of elbow is "..table.concat(elbow_velocity, ', '))
    -- 获取基座力/力矩
    base_force = getBaseForce()
    textmsg("The force of base is "..table.concat(base_force, ', '))
    -- 获取TCP目标位姿
    tcp_target_pose = getTcpTargetPose()
    textmsg("The pose of target tcp is "..table.concat(tcp_target_pose, ', '))
    -- 获取TCP目标速度
    tcp_target_speed = getTcpTargetSpeed()
    textmsg("The speed of target tcp is "..table.concat(tcp_target_speed, ', '))
    -- 获取TCP目标力/力矩
    tcp_target_force = getTcpTargetForce()
    textmsg("The force of target tcp is "..table.concat(tcp_target_force, ', '))
    -- 获取机械臂关节标志
    joint_state = getJointState()
    textmsg("The state of joint is "..table.concat(joint_state, ', '))
    -- 获取关节的伺服状态
    joint_servo_mode = getJointServoMode()
    textmsg("The mode of joint servo is "..table.concat(joint_servo_mode, ', '))
    -- 获取机械臂关节角度
    -- 对应AUBOPE joint_positions =get_current_waypoint
    joint_positions = getJointPositions()
    textmsg("The positions of joint are "..table.concat(joint_positions, ', '))
    -- 获取机械臂关节速度
    joint_speeds = getJointSpeeds()
    textmsg("The speeds of joint are "..table.concat(joint_speeds, ', '))
    -- 获取机械臂关节加速度
    joint_acc = getJointAccelerations()
    textmsg("The accelerations of joint are "..table.concat(joint_acc, ', '))
    -- 获取机械臂关节力矩
    joint_torque_sensors = getJointTorqueSensors()
    textmsg("The torque sensors of joint are "..table.concat(joint_torque_sensors, ', '))
    -- 获取底座力传感器读数
    base_force_sensor = getBaseForceSensor()
    textmsg("The force sensors of base are "..table.concat(base_force_sensor, ', '))
    -- 获取TCP力传感器读数
    tcp_force_sensors = getTcpForceSensors()
    textmsg("The force sensors of tcp are "..table.concat(tcp_force_sensors, ', '))
    -- 获取机械臂关节电流
    joint_currents = getJointCurrents()
    textmsg("The currents of joint are "..table.concat(joint_currents, ', '))
    -- 获取机械臂关节电压
    joint_voltages = getJointVoltages()
    textmsg("The voltages of joint are "..table.concat(joint_voltages, ', '))
    -- 获取机械臂关节温度
    joint_temperature = getJointTemperatures()
    textmsg("The temperatures of joint are "..table.concat(joint_temperature, ', '))
    -- 获取关节全球唯一ID
    joint_unique_id = getJointUniqueIds()
    textmsg("The unique id of joint is "..table.concat(joint_unique_id, ', '))
    -- 获取关节固件版本
    joint_firmware_ver = getJointFirmwareVersions()
    textmsg("The firmware version of joint is "..table.concat(joint_firmware_ver, ', '))
    -- 获取关节硬件版本
    joint_hardware_ver = getJointHardwareVersions()
    textmsg("The hardware version of joint is "..table.concat(joint_hardware_ver, ', '))
    -- 获取MasterBoard全球唯一ID
    master_board_unique_id = getMasterBoardUniqueId()
    textmsg("The unique id of master board is "..master_board_unique_id)
    -- 获取MasterBoard固件版本
    master_board_firmware_ver = getMasterBoardFirmwareVersion()
    textmsg("The firmware version of master board is "..master_board_firmware_ver)
    -- 获取MasterBoard硬件版本
    master_board_hardware_ver = getMasterBoardHardwareVersion()
    textmsg("The hardware version of master board is "..master_board_hardware_ver)
    -- 获取SlaveBoard全球唯一ID
    slave_board_unique_id = getSlaveBoardUniqueId()
    textmsg("The unique id of slave board is "..slave_board_unique_id)
    -- 获取SlaveBoard固件版本
    slave_board_firmware_ver = getSlaveBoardFirmwareVersion()
    textmsg("The firmware version of slave board is "..slave_board_firmware_ver)
    -- 获取SlaveBoard硬件版本
    slave_board_hardware_ver = getSlaveBoardHardwareVersion()
    textmsg("The hardware version of slave board is "..slave_board_hardware_ver)
    -- 获取工具端全球唯一ID
    tool_unique_id = getToolUniqueId()
    textmsg("The unique id of tool is "..tool_unique_id)
    -- 获取工具端固件版本
    tool_firmware_ver = getToolFirmwareVersion()
    textmsg("The firmware version of tool is "..tool_firmware_ver)
    -- 获取工具端硬件版本
    tool_hardware_ver = getToolHardwareVersion()
    textmsg("The hardware version of tool is "..tool_hardware_ver)
    -- 获取底座全球唯一ID
    pedestal_unique_id = getPedestalUniqueId()
    textmsg("The unique id of pedestal is "..pedestal_unique_id)
    -- 获取底座固件版本
    pedestal_firmware_ver = getPedestalFirmwareVersion()
    textmsg("The firmware version of pedestal is "..pedestal_firmware_ver)
    -- 获取底座硬件版本
    pedestal_hardware_ver = getPedestalHardwareVersion()
    textmsg("The hardware version of pedestal is "..pedestal_hardware_ver)
    -- 获取机械臂关节目标位置角度
    joint_target_pos = getJointTargetPositions()
    textmsg("The target positions of joint are "..table.concat(joint_target_pos, ', '))
    -- 获取机械臂关节目标速度
    joint_target_speed = getJointTargetSpeeds()
    textmsg("The target speeds of joint are "..table.concat(joint_target_speed, ', '))
    -- 获取机械臂关节目标加速度
    joint_target_acc = getJointTargetAccelerations()
    textmsg("The target accelerations of joint are "..table.concat(joint_target_acc, ', '))
    -- 获取机械臂关节目标力矩
    joint_target_torques = getJointTargetTorques()
    textmsg("The target torques of joint are "..table.concat(joint_target_torques, ', '))
    -- 获取机械臂关节目标电流
    joint_target_currents = getJointTargetCurrents()
    textmsg("The target currents of joint are "..table.concat(joint_target_currents, ', '))
    -- 获取控制柜温度
    control_box_temperature = getControlBoxTemperature()
    textmsg("The temperature of control box is "..control_box_temperature)
    -- 获取母线电压
    main_voltage = getMainVoltage()
    textmsg("The main voltage is "..main_voltage)
    -- 获取母线电流
    main_current = getMainCurrent()
    textmsg("The main current is "..main_current)
    -- 获取机器人电压
    robot_voltage = getRobotVoltage()
    textmsg("The voltage of robot is "..robot_voltage)
    -- 获取机器人电流
    robot_current = getRobotCurrent()
    textmsg("The current of robot is "..robot_current)
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
  require('compat_env').setfenv(p_example_robot_state, env)
  p_example_robot_state()
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
