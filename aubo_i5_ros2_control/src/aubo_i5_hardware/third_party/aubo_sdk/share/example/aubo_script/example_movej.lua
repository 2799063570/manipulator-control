--[[
    功能：关节运动
    描述：以关节运动的方式依次经过3个路点
]]
return function(api)
    local _ENV = require('aubo').sched.select_robot(1)
    local sched = require('aubo').sched
    sched.sleep(0)
    
    pi = 3.14159265358979323846
 
    -- 路点，用关节角表示，单位：弧度
    waypoint0_q = {0.0/180*pi, -15/180*pi, 100/180*pi, 25/180*pi, 90.0/180*pi, 0.0/180*pi}
    waypoint1_q = {35.92/180*pi, -11.28/180*pi, 59.96/180*pi, -18.76/180*pi, 90.0/180*pi, 35.92/180*pi}
    waypoint2_q = {41.04/180*pi, -7.65/180*pi, 98.80/180*pi, 16.44/180*pi, 90.0/180*pi, 11.64/180*pi}

    -- 设置机械臂的速度比率 
    setSpeedFraction(0.75)

    -- 关节运动
    moveJoint(waypoint0_q, 80/180*pi, 60/180*pi, 0, 0)

    moveJoint(waypoint1_q, 80/180*pi, 60/180*pi, 0, 0)

    moveJoint(waypoint2_q, 80/180*pi, 60/180*pi, 0, 0)
end

