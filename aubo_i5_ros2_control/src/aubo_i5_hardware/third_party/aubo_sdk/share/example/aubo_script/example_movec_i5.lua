--[[
    功能：圆周运动
    注意：本示例是根据型号i5而编写的
    步骤：
        第一步：关节运动到初始点，即路点0
        第二步：直线运动到圆的第一个点，即路点1
        第三步：计算圆周的第四个点，即另一半圆弧的中间点
        第四步：执行圆周运动
    注意：执行moveCircle接口，机械臂做圆弧运动。
    此处，一个完整的圆周运动，是由两个圆弧运动拼接而成的。
]]
return function(api)
    local _ENV = require('aubo').sched.select_robot(1)
    local sched = require('aubo').sched
    sched.sleep(0)
    
    pi = 3.14159265358979323846
     
    -- 设置工具中心点（TCP）
    setTcpOffset({0,0,0,0,0,0})
 
    -- 路点0，用关节角表示，单位：弧度
    waypoint0_q = {0.0/180*pi, -15/180*pi, 100/180*pi, 25/180*pi, 90.0/180*pi, 0.0/180*pi}
    -- 关节运动到初始点，即路点0
    moveJoint(waypoint0_q, 80/180*pi, 60/180*pi, 0, 0)
     
    -- 路点1，用位姿(x,y,z,rx,ry,rz)表示，位置的单位：米，姿态的单位：弧度
    waypoint1_p = {0.54887, -0.1215, 0.563, 3.142, 0, 1.571}
    -- 直线运动到圆的第一个点，即路点1
    moveLine(waypoint1_p, 1.2, 0.25, 0, 0)
     
    -- 设置圆的路径模式为无拘束模式，
    -- 即工具姿态呈线性变化，绕空间定轴转动，从起点姿态变化到目标点姿态
    setCirclePathMode(0)
     
    -- 路点2，用位姿(x,y,z,rx,ry,rz)表示，位置的单位：米，姿态的单位：弧度
    waypoint2_p = {0.54887, 0.18830, 0.5703, 3.142, 0, 1.571}
     
    -- 路点3，用位姿(x,y,z,rx,ry,rz)表示，位置的单位：米，姿态的单位：弧度
    waypoint3_p = {0.54887, 0.18830, 0.3055, 3.142, 0, 1.571}
    
    -- 计算圆周的第四个点，即另一半圆弧的中间点
    waypoint4_p = calculateCircleFourthPoint(waypoint1_p, waypoint2_p, waypoint3_p, 1)
    
    -- 执行圆周运动的圈数
    for i = 1, 3 do
        moveCircle(waypoint2_p, waypoint3_p, 1.2, 0.25, 0, 0)
		moveCircle(waypoint4_p, waypoint1_p, 1.2, 0.25, 0, 0)
    end
end