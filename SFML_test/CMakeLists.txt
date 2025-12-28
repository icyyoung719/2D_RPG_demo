# 获取当前子CMake文件所在目录的完整路径
get_filename_component(CURRENT_DIR_PATH "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
# 从完整路径中提取目录名称
get_filename_component(CURRENT_DIR_NAME "${CURRENT_DIR_PATH}" NAME)
project(${CURRENT_DIR_NAME})

message(STATUS "SFML_DIR is: ${SFML_DIR}")

# Use static linking for SFML
set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 3.0 REQUIRED COMPONENTS Window Graphics System)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/dependency
)

file(GLOB SOURCES "src/*.cpp")
file(GLOB DEPENDENCY_SOURCES "dependency/*.cpp")

set(ALL_SOURCES ${SOURCES} ${DEPENDENCY_SOURCES})

add_executable(${PROJECT_NAME} ${ALL_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)
if (SFML_FOUND)
    message(STATUS "SFML found!")
else()
    message(FATAL_ERROR "SFML not found!")
endif()

# 设置为控制台子系统（仅 Windows 下适用）
if (MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
endif()
