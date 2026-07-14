--[[
    功能：机械臂正逆解
    步骤：
        第一步：逆解
        第二部：正解
]]
local aubo = require('aubo')
local sched = sched or aubo.sched
local math = aubo.math or math

local sleep = sched.sleep
local thread = sched.thread
local sync = sched.sync
local run = sched.run
local kill = sched.kill
function p_example_algorithm()
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
    setPlanContext(0, 2, "example_algorithm.lua")
    -- 参考关节角，单位：弧度
    waypoint_0_q = {0.00123059, -0.26063, 1.7441, 0.437503,   1.56957,  0.00107586}
    -- 目标位姿
    waypoint_1_p = {0.549273, -0.12094,   0.464501, 3.13802,  0.00122674, 1.57095}
    -- 逆解
    -- 对应AUBOPE joint=get_target_pose({0.549273, -0.12094,0.464501}, rpyToQuaternion({3.13802,  0.00122674, 1.57095}),false,{0.0003,0.0001,0.00066},{1,0,0,0})
    waypoint_1_q = inverseKinematics(waypoint_0_q, waypoint_1_p)
    textmsg("The result of inverse kinematics is "..table.concat(waypoint_1_q, ', '))
    -- 正解
    waypoint_2_p = forwardKinematics(waypoint_1_q)
    textmsg("The result of forward kinematics is "..table.concat(waypoint_2_p, ', '))

    -- 对应AUBOPE set_global_variable ("V_I_b", 34)
    setInt32("V_I_b", 34);
    sleep(1)
    -- 对应AUBOPE get_global_variable ("V_I_b")
    textmsg("V_I_b is "..getInt32("V_I_b", 0))

    -- 对应AUBOPE set_global_variable ("custom1", true)
    setBool("custom1", true)
    sleep(1)
    -- 对应AUBOPE get_global_variable ("custom1")
    custom1 =getBool("custom1", true)
    textmsg("custom1 is "..tostring(custom1))

    -- 对应AUBOPE set_global_variable ("custom3", 0.1)
    setFloat("custom3", 0.1)
    sleep(1)
    -- 对应AUBOPE get_global_variable ("custom3")
    textmsg("custom3 is "..getFloat("custom3", 0))

    -- 对应AUBOPE set_global_variable ("custom4", 0.2)
    setDouble("custom4", 0.2)
    sleep(1)
    -- 对应AUBOPE get_global_variable ("custom4")
    textmsg("custom4 is "..getDouble("custom4", 0))

    -- 对应AUBOPE set_global_variable ("custom5", "test")
    setString("custom5", "test")
    sleep(1)
    -- 对应AUBOPE get_global_variable ("custom5")
    textmsg(getString("custom5", ""))

    -- 对应AUBOPE init_global_variables("custom4")
    clearNamedVariable("custom4")
    textmsg("custom4 is "..getDouble("custom4", 0))

    --添加保持寄存器(写)
    modbusAddSignal( "127.0.0.1,1504", 1, 0x00, 0x05, "READ_HOLDING_REGISTER_W_00", true)
    --设置频率
    modbusSetSignalUpdateFrequency("READ_HOLDING_REGISTER_W_00", 1)
    --写保持寄存器
    -- 对应AUBOPE set_modbus_io_status("READ_HOLDING_REGISTER_W_00",1)
    modbusSetOutputSignal("READ_HOLDING_REGISTER_W_00",0x01)
    --删除
    modbusDeleteSignal("READ_HOLDING_REGISTER_W_00")

    --添加保持寄存器(读)
    modbusAddSignal( "127.0.0.1,1504", 1, 0x00, 0x03, "READ_HOLDING_REGISTER_R_00", true)
    --设置频率
    modbusSetSignalUpdateFrequency("READ_HOLDING_REGISTER_R_00", 1)
    --读写保持寄存器
    --AUBOPE get_modbus_io_status("READ_HOLDING_REGISTER_R_00")
    textmsg("READ_HOLDING_REGISTER_R_00 is "..modbusGetSignalStatus( "READ_HOLDING_REGISTER_R_00"))
    --删除
    modbusDeleteSignal("READ_HOLDING_REGISTER_R_00")
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
  require('compat_env').setfenv(p_example_algorithm, env)
  p_example_algorithm()
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
