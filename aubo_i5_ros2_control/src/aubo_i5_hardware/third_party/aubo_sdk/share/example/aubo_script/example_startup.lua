-- 示例功能： 机械臂上电与断电
-- 引入 aubo 模块并选择机器人
local _ENV = require('aubo').sched.select_robot(1)
local sched = require('aubo').sched
-- 等待机器人当前的模式类型变成目标模式类型
local function wait_for_target_mode(target_robot_mode_value, target_robot_mode_key)
    -- 等待机器人达到目标状态
    while true do
        if getSafetyModeType() == SafetyModeType.Fault then
            -- 如果当前安全模式为故障，则输出错误信息并返回 -1
            textmsg("Error! The current safety mode is Fault!")
            return -1 
        else
            local current_robot_mode = getRobotModeType()
            if current_robot_mode == target_robot_mode_value then
                -- 如果当前机器人模式等于目标模式，则输出信息并返回 0
                textmsg("The robot's current mode changes to the target mode: " .. target_robot_mode_key)
                return 0
            else
                sched.sleep(1)
            end
        end
    end
end
-- 主函数
local function main(api)
    local mass = 0.0                               -- 负载质量，单位kg
    local cog = {0.0, 0.0, 0.0}                    -- 负载重心，单位m
    local aom = {0.0, 0.0, 0.0}
    local inertia = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0} -- 负载惯量
    -- 设置机器人的负载参数
    setPayload(mass, cog, aom, inertia)
    sched.sleep(0.1)
    -- 获取机器人的负载参数
    mass, cog, aom, inertia = getPayload()
    textmsg("The mass of the robot is " .. mass)
    textmsg("The cog of the robot is {" .. table.concat(cog, ', ') .. "}")
    if getRobotModeType() == RobotModeType.Running then
        -- 如果机器人当前模式为运行，则输出信息
        textmsg("The robot's current state is Running.")
        textmsg("The robot has released the brake.")
        sched.sleep(0.1)
    else
        -- 机器人上电
        poweron()
        -- 等待机器人模式变为空闲
        if wait_for_target_mode(RobotModeType.Idle, "Idle") == -1 then
            textmsg("The robot failed to power on!")
            return
        end
        textmsg("Robot powered on successfully!")
        -- 机器人松刹车
        startup()
        -- 等待机器人模式变为运行
        if wait_for_target_mode(RobotModeType.Running, "Running") == -1 then
            textmsg("The robot failed to release the brake!")
            return
        end
        textmsg("Robot released the brake successfully!")
    end
    -- 机器人断电
    poweroff()
    if wait_for_target_mode(RobotModeType.PowerOff, "PowerOff") == -1 then
        textmsg("The robot failed to poweroff!")
        return
    end
    textmsg("Robot powered off successfully!")  
end
-- 调用主函数
main()

