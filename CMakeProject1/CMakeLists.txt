cmake_minimum_required(VERSION 3.10)

# 获取当前子CMake文件所在目录的完整路径
get_filename_component(CURRENT_DIR_PATH "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
# 从完整路径中提取目录名称
get_filename_component(CURRENT_DIR_NAME "${CURRENT_DIR_PATH}" NAME)

project(${CURRENT_DIR_NAME})

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# cmake_policy(SET CMP0167 NEW)
find_package(Boost REQUIRED COMPONENTS system)

file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# 将源代码添加到此项目的可执行文件。
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${CPP_FILES})

# 添加调试信息
set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "ProgramDatabase")

# 设置为控制台子系统
if (MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
endif()

# TODO: 如有需要，请添加测试并安装目标。