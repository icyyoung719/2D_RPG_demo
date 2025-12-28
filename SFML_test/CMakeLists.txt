# 获取当前子CMake文件所在目录的完整路径
get_filename_component(CURRENT_DIR_PATH "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
# 从完整路径中提取目录名称
get_filename_component(CURRENT_DIR_NAME "${CURRENT_DIR_PATH}" NAME)

project(${CURRENT_DIR_NAME})

# Use static linking for SFML
set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 3.0 REQUIRED COMPONENTS Window Graphics System)

# 收集所有src和deps下的.cpp源文件和.h头文件
file(GLOB_RECURSE SRC_FILES src/*.cpp)
file(GLOB_RECURSE HEADER_FILES src/*.h)
file(GLOB_RECURSE DEPS_SRC_FILES deps/*/*.cpp)
file(GLOB_RECURSE DEPS_HEADER_FILES deps/*/*.h)

set(ALL_SOURCES
    ${SRC_FILES}
    ${HEADER_FILES}
    ${DEPS_SRC_FILES}
    ${DEPS_HEADER_FILES}
)

# 让头文件在IDE里分组显示
source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${ALL_SOURCES}
)

add_executable(${PROJECT_NAME} ${ALL_SOURCES})

# 只把 src, deps/json, deps/log 加入头文件查找目录
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/deps
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/json
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/log
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

# 设置为控制台子系统（仅 Windows 下适用）
if (MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
endif()