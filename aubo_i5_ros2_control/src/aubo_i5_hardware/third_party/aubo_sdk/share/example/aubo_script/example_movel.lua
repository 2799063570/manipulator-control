--[[
    功能：直线运动
    步骤：
        第一步：关节运动到初始位置
        第二步：以直线运动的方式依次走3个路点 Waypoint_1_p、Waypoint_2_p和Waypoint_3_p
]]
local aubo = require('aubo')
local sched = sched or aubo.sched
local math = aubo.math or math

local sleep = sched.sleep
local thread = sched.thread
local sync = sched.sync
local run = sched.run
local kill = sched.kill
function p_example_movel()
    -- 功能：恢复所有数字输入、数字输出为默认值
    setDigitalInputActionDefault()
    setDigitalOutputRunstateDefault()
    
    -- 功能：设置末端负载、TCP
    setPayload(0, {0,0,0}, {0,0,0}, {0,0,0,0,0,0})
    setTcpOffset({0.0000,0.0000,0.0000,0,0,0})
    Base = {0,0,0,0,0,0}
    Tool = {0,0,0,0,0,0}
    
    -- 关节运动到初始位置
    q = {-2.40194, 0.103747, 1.7804, 0.108636, 1.57129, -2.6379}
    -- 对应AUBOPE move_joint(q,false)
    moveJoint(q, 1, 1, 0, 0)
    textmsg("moving to initial position")

    Waypoint_1_p = {-0.155944, -0.727344, 0.439066, 3.05165, 0.0324355, 1.80417}
    Waypoint_2_p = {-0.581143, -0.357548, 0.439066, 3.05165, 0.0324355, 1.80417}
    Waypoint_3_p = {0.503502, -0.420646, 0.439066, 3.05165, 0.0324355, 1.80417}
    
    -- 以直线运动的方式依次走3个路点： Waypoint_1、Waypoint_2和Waypoint_3
    -- 对应AUBOPE move_line(q,false)
    moveLine(Waypoint_1_p, 1, 1, 0, 0)
    textmsg("line move to waypoint_1")

    moveLine(Waypoint_2_p, 1, 1, 0, 0)
    textmsg("line move to waypoint_2")

    moveLine(Waypoint_3_p, 1, 1, 0, 0)
    textmsg("line move to waypoint_3")
    
end

local plugin = {
  PRIORITY = 1000, -- set the plugin priority, which determines plugin execution order
  VERSION = "0.1",
  VENDOR = "Aubo Robotics",
}

function plugin:start_robot1(env)
  print("start_robot1---")
  -- 配置脚本环境
  local _ENV = env
  for k,v in pairs(self.api:loadf(1)) do _ENV[k] = v end
  p_example_movel()
end

function plugin:start(api)
  --
  self.api = api
  print("start---")
  self:start_robot1(_ENV)
end

function plugin:robot_error_handler(name, err)
  --
  print("An error hanppen to robot "..name)
end

-- return our plugin object
return plugin
