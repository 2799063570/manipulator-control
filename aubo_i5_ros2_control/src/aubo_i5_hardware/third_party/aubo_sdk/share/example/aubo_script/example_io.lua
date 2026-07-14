--[[
    功能：设置和获取IO相关配置
]]
local aubo = require('aubo')
local sched = sched or aubo.sched
local math = aubo.math or math

local sleep = sched.sleep
local thread = sched.thread
local sync = sched.sync
local run = sched.run
local kill = sched.kill
function p_example_io()
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
    setPlanContext(0, 2, "example_io.lua")
    
    -- 获取标准数字IO的输入和输出数量
    standard_digital_input_num = getStandardDigitalInputNum()
    textmsg("standard_digital_input_num: "..standard_digital_input_num)
    standard_digital_output_num = getStandardDigitalOutputNum()
    textmsg("standard_digital_output_num: "..standard_digital_output_num)
    -- 设置所有的输入触发动作为 Default
    setDigitalInputActionDefault()
    -- 获取输入触发动作
    standard_input_action1 = getStandardDigitalInputAction(0x00000001)
    textmsg("standard digital input action: "..standard_input_action1)
    -- 设置标准数字输入触发动作为 Freedrive
    setStandardDigitalInputAction(0x00000001, 1)
    sleep(1)
    -- 获取输入触发动作
    standard_input_action2 = getStandardDigitalInputAction(0x00000001)
    textmsg("standard digital input action: "..standard_input_action2)
    -- 设置所有的输出状态选择为 None
    setDigitalOutputRunstateDefault()
    -- 获取标准数字输出状态选择
    standard_digital_output_runstate1 = getStandardDigitalOutputRunstate(0x00000001)
    textmsg("standard digital output runstate: "..standard_digital_output_runstate1)
    -- 设置标准数字输出状态选择为 STOP_HIGH
    setStandardDigitalOutputRunstate(0x00000001, 2)
    sleep(1)
    -- 获取标准数字输出状态选择
    standard_digital_output_runstate2 = getStandardDigitalOutputRunstate(0x00000001)
    textmsg("standard digital output runstate: "..standard_digital_output_runstate2)
    -- 获取所有的标准数字输入值
    input_value = {}
    for i = 1, 16, 1 do
      input_value[i] = tostring(getStandardDigitalInput(i-1))
    end 
    textmsg("The standard digital input values are "..table.concat(input_value, ', '))
    -- 获取所有的标准数字输出值
    output_value = {}
    for i = 1, 16, 1 do
      output_value[i] = tostring(getStandardDigitalOutput(i-1))
    end 
    textmsg("The standard digital output values are "..table.concat(output_value, ', '))

    -- 设置标准数字输出
    -- 对应AUBOPE set_robot_io_status(RobotIOType.RobotBoardUserDO, "U_DO_00", 1)
    setStandardDigitalOutput(0,true)
    --获取标准数字输出值
    -- 对应AUBOPE get_robot_io_status (RobotIOType.RobotBoardUserDO, "U_DO_00")
    textmsg("U_DO_00 is "..tostring(getStandardDigitalOutput(0)))
    --获取标准数字输入值
    --get_robot_io_status(RobotIOType.RobotBoardUserDI,"U_DI_00")
    textmsg("U_DI_00 is "..tostring(getStandardDigitalInput(0)))



    -- 获取标准模拟输入数量
    standard_analog_input_num = getStandardAnalogInputNum()
    textmsg("standard analog input number: ".. standard_analog_input_num)
    -- 获取标准模拟输出数量
    standard_analog_output_num = getStandardAnalogOutputNum()
    textmsg("standard analog output number: "..standard_analog_output_num)
    -- 设置标准模拟输入范围
    setStandardAnalogInputDomain(0, 13)
    sleep(1)
    -- 获取标准模拟输出范围
    standard_analog_input_domain = getStandardAnalogInputDomain(0)
    textmsg("standard analog input domain: "..standard_analog_input_domain)
    -- 设置标准模拟输出范围
    setStandardAnalogOutputDomain(0, 12)
    sleep(1)
    -- 获取标准模拟输出范围
    standard_analog_output_domain = getStandardAnalogOutputDomain(0)
    textmsg("standard analog output domain: "..standard_analog_output_domain)
    -- 设置标准模拟输出状态选择为 RUNNING
    setStandardAnalogOutputRunstate(0, 3)
    sleep(1)
    -- 获取标准模拟输出状态选择
    standard_output_runstate = getStandardAnalogOutputRunstate(0)
    textmsg("standard output runstate: "..standard_output_runstate)

    -- 设置标准模拟输出
    -- 对应AUBOPE set_robot_io_status(RobotIOType.RobotBoardUserAO, "U_VO_00", 3)
    setStandardAnalogOutput(0,3)
    sleep(1)
    --获取标准模拟输出值
    -- 对应AUBOPE get_robot_io_status (RobotIOType.RobotBoardUserAO, "U_VO_00")
    textmsg("U_VO_00 is "..tostring(getStandardAnalogOutput(0)))
    --获取标准模拟输入值
    --get_robot_io_status(RobotIOType.RobotBoardUserAI,"U_VI_00")
    textmsg("U_VI_00 is "..tostring(getStandardAnalogInput(0)))


    -- 获取工具数字IO输入数量
    tool_digital_input_num = getToolDigitalInputNum()
    textmsg("tool digital input number: "..tool_digital_input_num)
    -- 获取工具数字IO输出数量
    tool_digital_output_num = getToolDigitalOutputNum()
    textmsg("tool digital output number: "..tool_digital_output_num)
    -- 设置指定的工具端IO为输入
    setToolIoInput(1, true)
    sleep(1)
    -- 判断指定的工具端IO是否为输入
    isInput = isToolIoInput(1)
    textmsg("Is it the input: "..tostring(isInput))
    -- 获取工具端数字IO输入
    -- 对应AUBOPE get_robot_io_status (RobotIOType.RobotToolDI, "T_DI_01")
    tool_digital_input = getToolDigitalInput(1)
    textmsg("tool digital input: "..tostring(tool_digital_input))
    textmsg("T_DI_01 is "..tostring(tool_digital_input))
    -- 获取工具端模拟输入数量
    tool_analog_input_num = getToolAnalogInputNum()
    textmsg("tool analog input number: "..tool_analog_input_num)
    -- 获取工具端模拟输入
    -- 对应AUBOPE get_robot_io_status (RobotIOType.RobotToolAI, "T_AI_00")
    tool_analog_input_value = getToolAnalogInput(0)
    textmsg("tool analog input value: "..tool_analog_input_value)
    textmsg("T_AI_00 is "..tool_analog_input_value)

    -- 设置指定的工具端IO为输出
    setToolIoInput(3, false)
    -- 判断指定的工具端IO是否为输入
    isInput = isToolIoInput(3)
    textmsg("T_DO_03 it the input: "..tostring(isInput))
    -- 设置工具端数字IO输出
    -- 对应AUBOPE set_robot_io_status(RobotIOType.RobotToolDO, "T_DO_03",false)
    getToolDigitalOutput(3,false)
    -- 获取工具端数字IO输出
    -- 对应AUBOPE get_robot_io_status(RobotIOType.RobotToolDO, "T_DO_03")
    textmsg("T_DO_03 is "..tostring(getToolDigitalOutput(3)))
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
  require('compat_env').setfenv(p_example_io, env)
  p_example_io()
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
