cmake_minimum_required(VERSION 3.16)
project(aubo_sdk_example)
set(CMAKE_CXX_STANDARD 17)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
macro(IMPORT_SDK SDK)
    if(DEFINED USING_SDK_SOURCE_CODE)
        message(STATUS "Use aubo_sdk source code.")
        set(SDK_LIB ${ARGV0})
        if("${ARGV1}" STREQUAL "robot_proxy")
            set(PROXY_LIB ${ARGV1})
        endif()
    else()
        string(REGEX REPLACE "(.+)\\share/example.*" "\\1" AUBO_SDK_PATH ${PROJECT_SOURCE_DIR})
        message(STATUS "FOUND AUBO_SDK PATH ${AUBO_SDK_PATH}")
        set(CMAKE_PREFIX_PATH ${AUBO_SDK_PATH})
        find_package(aubo_sdk REQUIRED aubo_sdk)
        message(STATUS "Import aubo_sdk::${ARGV0}")
        set(SDK_LIB aubo_sdk::${ARGV0})
        if("${ARGV1}" STREQUAL "robot_proxy")
            message(STATUS "Import aubo_sdk::${ARGV1}")
            set(PROXY_LIB aubo_sdk::${ARGV1})
        endif()
    endif()
endmacro(IMPORT_SDK)

find_package(Qt5 COMPONENTS Widgets)
if(Qt5_FOUND)
    IMPORT_SDK(aubo_sdk robot_proxy)
    add_subdirectory(robot_proxy EXCLUDE_FROM_ALL)
else()
    IMPORT_SDK(aubo_sdk)
endif()

add_subdirectory(aubope EXCLUDE_FROM_ALL)

find_package(Threads REQUIRED)
if (NOT EXECUTABLE_OUTPUT_PATH)
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/bin)
endif()

# example_armplay如果以库的形式编译，需自己手动安装 nlohmann_json
if(DEFINED USING_SDK_SOURCE_CODE)
add_executable(example_armplay armplay.cpp)
target_link_libraries(example_armplay ${SDK_LIB} nlohmann_json::nlohmann_json Threads::Threads)
add_custom_command(
    TARGET example_armplay POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_SOURCE_DIR}/trajs/ ${EXECUTABLE_OUTPUT_PATH}/../trajs)
endif()

add_executable(example_servoj example_servoj.cpp)
target_link_libraries(example_servoj ${SDK_LIB} Threads::Threads)

add_executable(example_servoj2 example_servoj2.cpp)
target_link_libraries(example_servoj2 ${SDK_LIB} Threads::Threads)

add_executable(example_servo_cartesian example_servo_cartesian.cpp)
target_link_libraries(example_servo_cartesian ${SDK_LIB} Threads::Threads)

add_executable(example_trackj example_trackj.cpp)
target_link_libraries(example_trackj ${SDK_LIB} Threads::Threads)

add_executable(example_spline example_spline.cpp)
target_link_libraries(example_spline ${SDK_LIB} Threads::Threads)

add_executable(example_record record.cpp)
target_link_libraries(example_record ${SDK_LIB} Threads::Threads)

add_executable(example_offline_track offline_track.cpp)
target_link_libraries(example_offline_track ${SDK_LIB} Threads::Threads)

add_executable(example_extract_data example_extract_data.cpp)
target_link_libraries(example_extract_data ${SDK_LIB}  Threads::Threads)

add_custom_command(
    TARGET example_servoj POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_SOURCE_DIR}/trajs/ ${EXECUTABLE_OUTPUT_PATH}/../trajs)

add_custom_command(
    TARGET example_servoj2 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_SOURCE_DIR}/trajs/ ${EXECUTABLE_OUTPUT_PATH}/../trajs)

add_custom_command(
    TARGET example_spline POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_SOURCE_DIR}/trajs/ ${EXECUTABLE_OUTPUT_PATH}/../trajs)

add_custom_command(
    TARGET example_record POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_SOURCE_DIR}/trajs/ ${EXECUTABLE_OUTPUT_PATH}/../trajs)

add_custom_command(
    TARGET example_offline_track POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_SOURCE_DIR}/trajs/ ${EXECUTABLE_OUTPUT_PATH}/../trajs)

add_custom_command(
    TARGET example_extract_data POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_SOURCE_DIR}/trajs/ ${EXECUTABLE_OUTPUT_PATH}/../trajs)

add_executable(example_io example_io.cpp)
target_link_libraries(example_io ${SDK_LIB} Threads::Threads)

add_executable(example_movej example_movej.cpp)
target_link_libraries(example_movej ${SDK_LIB} Threads::Threads)

add_executable(example_axis_move_joint example_axis_move_joint.cpp)
target_link_libraries(example_axis_move_joint ${SDK_LIB} Threads::Threads)

add_executable(example_axis_offline example_axis_offline.cpp)
target_link_libraries(example_axis_offline ${SDK_LIB} Threads::Threads)

add_executable(example_movel_i5 example_movel_i5.cpp)
target_link_libraries(example_movel_i5 ${SDK_LIB} Threads::Threads)

add_executable(example_movel_singularity_i5 example_movel_singularity_i5.cpp)
target_link_libraries(example_movel_singularity_i5 ${SDK_LIB} Threads::Threads)

add_executable(example_rtde example_rtde.cpp)
target_link_libraries(example_rtde ${SDK_LIB} Threads::Threads)

add_executable(example_startup example_startup.cpp)
target_link_libraries(example_startup ${SDK_LIB} Threads::Threads)

add_executable(example_freedrive example_freedrive.cpp)
target_link_libraries(example_freedrive ${SDK_LIB} Threads::Threads)

add_executable(example_multi_thread example_multi_thread.cpp)
target_link_libraries(example_multi_thread ${SDK_LIB} Threads::Threads)

add_executable(example_register example_register.cpp)
target_link_libraries(example_register ${SDK_LIB} Threads::Threads)

add_executable(example_math example_math.cpp)
target_link_libraries(example_math ${SDK_LIB} Threads::Threads)

add_executable(example_set_log_handler example_set_log_handler.cpp)
target_link_libraries(example_set_log_handler ${SDK_LIB} Threads::Threads)

add_executable(example_movec_i5 example_movec_i5.cpp)
target_link_libraries(example_movec_i5 ${SDK_LIB} Threads::Threads)

add_executable(example_config example_config.cpp)
target_link_libraries(example_config ${SDK_LIB} Threads::Threads)

add_executable(example_config2 example_config2.cpp)
target_link_libraries(example_config2 ${SDK_LIB} Threads::Threads)

add_executable(example_state example_state.cpp)
target_link_libraries(example_state ${SDK_LIB} Threads::Threads)

add_executable(example_script example_script.cpp)
target_link_libraries(example_script ${SDK_LIB} Threads::Threads)

add_executable(example_timer example_timer.cpp)
target_link_libraries(example_timer ${SDK_LIB} Threads::Threads)

add_executable(example_force_control example_force_control.cpp)
target_link_libraries(example_force_control ${SDK_LIB} Threads::Threads)

add_executable(example_force_control2 example_force_control2.cpp)
target_link_libraries(example_force_control2 ${SDK_LIB} Threads::Threads)

add_executable(example_spline2 example_spline2.cpp)
target_link_libraries(example_spline2 ${SDK_LIB} Threads::Threads)

add_executable(example_collision_recovery example_collision_recovery.cpp)
target_link_libraries(example_collision_recovery ${SDK_LIB}  Threads::Threads)

add_executable(example_move_speed example_move_speed.cpp)
target_link_libraries(example_move_speed ${SDK_LIB}  Threads::Threads)

add_executable(example_algorithm example_algorithm.cpp)
target_link_libraries(example_algorithm ${SDK_LIB}  Threads::Threads)

add_executable(example_payload_identify example_payload_identify.cpp)
target_link_libraries(example_payload_identify ${SDK_LIB}  Threads::Threads)

add_executable(example_payload_identify1 example_payload_identify1.cpp)
target_link_libraries(example_payload_identify1 ${SDK_LIB}  Threads::Threads)

add_executable(example_rtde_uds example_rtde_uds.cpp)
target_link_libraries(example_rtde_uds ${SDK_LIB}  Threads::Threads)

add_executable(example_script_uds example_script_uds.cpp)
target_link_libraries(example_script_uds ${SDK_LIB}  Threads::Threads)

add_executable(example_rtde_udp example_rtde_udp.cpp)
target_link_libraries(example_rtde_udp ${SDK_LIB}  Threads::Threads)

add_executable(example_script2 example_script2.cpp)
target_link_libraries(example_script2 ${SDK_LIB} Threads::Threads)

add_executable(example_data_collect example_data_collect.cpp)
target_link_libraries(example_data_collect ${SDK_LIB} Threads::Threads)

add_executable(example_fc_assemble example_fc_assemble.cpp)
target_link_libraries(example_fc_assemble ${SDK_LIB} Threads::Threads)

add_executable(example_fc_assemble_usercoord example_fc_assemble_usercoord.cpp)
target_link_libraries(example_fc_assemble_usercoord ${SDK_LIB} Threads::Threads)

add_executable(example_moveSpiral_i5 example_moveSpiral_i5.cpp)
target_link_libraries(example_moveSpiral_i5 ${SDK_LIB} Threads::Threads)

add_executable(example_movel_offset example_movel_offset.cpp)
target_link_libraries(example_movel_offset ${SDK_LIB} Threads::Threads)

add_executable(example_move_speed2 example_move_speed2.cpp)
target_link_libraries(example_move_speed2 ${SDK_LIB}  Threads::Threads)

add_executable(example_run_program example_run_program.cpp)
target_link_libraries(example_run_program ${SDK_LIB}  Threads::Threads)

add_executable(example_popup_dismiss example_popup_dismiss.cpp)
target_link_libraries(example_popup_dismiss ${SDK_LIB}  Threads::Threads)

add_executable(example_step_mode example_step_mode.cpp)
target_link_libraries(example_step_mode ${SDK_LIB}  Threads::Threads)

add_executable(example_pause_resume example_pause_resume.cpp)
target_link_libraries(example_pause_resume ${SDK_LIB}  Threads::Threads)

