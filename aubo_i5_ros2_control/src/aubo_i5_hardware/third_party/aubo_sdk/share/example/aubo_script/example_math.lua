--[[
    功能：欧拉角与四元数转化
    步骤：
        第一步：欧拉角转四元数
        第二步：四元数转四元数
]]
local aubo = require('aubo')
local sched = sched or aubo.sched
local math = aubo.math or math

local sleep = sched.sleep
local thread = sched.thread
local sync = sched.sync
local run = sched.run
local kill = sched.kill
function p_example_math()
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
    setPlanContext(0, 2, "example_math.lua")
    -- 欧拉角转四元数
    -- 对应AUBOPE rpy2quaternion({0.835, 0.0780, 0.452, 0.305})
    rpy1 = {0.611, 0.785, 0.960}
    quat1 = rpyToQuaternion(rpy1)
    textmsg("The quaternion is "..table.concat(quat1, ','))
    -- 四元数转欧拉角
    -- 对应AUBOPE quaternion2rpy({0.835, 0.0780, 0.452, 0.305})
    quat2 = {0.835, 0.0780, 0.452, 0.305}
    rpy2 = quaternionToRpy(quat2)
    textmsg("The rpy is "..table.concat(rpy2, ', '))

    -- 对应AUBOPE sqrt(9)
    textmsg("The sqrt(9) is "..math.sqrt(9))
    -- 对应AUBOPE pow(10,2)
    textmsg("The pow(10,2) is "..math.pow(10,2))
    -- 对应AUBOPE ceil(3.2)
    textmsg("The ceil(3.2) is "..math.ceil(3.2))
    -- 对应AUBOPE floor(3.2))
    textmsg("The floor(3.2) is "..math.floor(3.2))
    -- 对应AUBOPE cos(3.14)
    textmsg("The cos(3.14) is "..math.cos(3.14))
    -- 对应AUBOPE sin(3.14)
    textmsg("The sin(3.14) is "..math.sin(3.14))
    -- 对应AUBOPE tan(0)
    textmsg("The tan(0) is "..math.tan(0))
    -- 对应AUBOPE acos(1)
    textmsg("The acos(1) is "..math.acos(1))
    -- 对应AUBOPE asin(1)
    textmsg("The asin(1) is "..math.asin(1))
    -- 对应AUBOPE atan(1)
    textmsg("The atan(1) is "..math.atan(1))
    -- 对应AUBOPE atan2(1,1)
    textmsg("The atan2(1,1) is "..math.atan2(1,1))
    -- 对应AUBOPE log(10,1000)
    --textmsg("The log(10,1000) is "..math.log(10,1000))
    -- 对应AUBOPE d2r(10)
    --textmsg("The d2r(10) is "..d2r(10))
    -- 对应AUBOPE r2d(10)
    --textmsg("The r2d(10) is "..r2d(10))

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
  require('compat_env').setfenv(p_example_math, env)
  p_example_math()
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
