local aubo = require('aubo')
local sched = sched or aubo.sched
local math = aubo.math or math

local sleep = sched.sleep
local thread = sched.thread
local sync = sched.sync
local run = sched.run
local kill = sched.kill
function p_example_server()
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
    setPlanContext(0, 2, "example_client.lua")
    -- server.lua
    local socket = require("socket")
    local host = "127.0.0.1"
    local port = 1234
    --对应AUBOPE　tcp.server.listen (port)
    local server = assert(socket.bind(host, port));
    while 1 do
      textmsg("server: waiting for client connection...");
      --等待client 连接 对应AUBOPE tcp.server.is_connected(ip_client) == true
      control = assert(server:accept());
      if control then
        textmsg("A client successfully connect!") 
      end
      while 1 do
        --对应AUBOPE recv=tcp.server.recv_str_data(ip_client)
        command, emsg = control:receive();
        if emsg == "closed" then
            control:close()
            --对应AUBOPE tcp.server.close()
            server:close()
            break
        end
        textmsg(command);
        --对应AUBOPE tcp.server.send_str_data(ip_client, "connect succ")
        assert(control:send(command));
     end
   end
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
  require('compat_env').setfenv(p_example_server, env)
  p_example_server()
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
