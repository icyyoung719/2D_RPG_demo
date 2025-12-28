# 获取当前子CMake文件所在目录的完整路径
get_filename_component(CURRENT_DIR_PATH "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
# 从完整路径中提取目录名称
get_filename_component(CURRENT_DIR_NAME "${CURRENT_DIR_PATH}" NAME)
project(${CURRENT_DIR_NAME})

# 设置 SFML CMake 配置路径
# set(SFML_DIR "D:/WPS_course/thirdparty_install/SFML-3.0.0/lib/cmake/SFML")

# 查找 SFML 库 (2.x or 3.x)
find_package(SFML 2.5 COMPONENTS window graphics system)

# 添加库文件路径
# link_directories(D:/WPS_course/thirdparty_install/SFML-3.0.0/lib)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/dependency
)

file(GLOB SOURCES "src/*.cpp")
file(GLOB DEPENDENCY_SOURCES "dependency/*.cpp")

set(ALL_SOURCES ${SOURCES} ${DEPENDENCY_SOURCES})

add_executable(${PROJECT_NAME} ${ALL_SOURCES})

# 链接 SFML 库
if(SFML_FOUND)
    if(SFML_VERSION_MAJOR EQUAL 3)
        target_link_libraries(${PROJECT_NAME}
            SFML::Graphics
            SFML::Window
            SFML::System
        )
    else()
        target_link_libraries(${PROJECT_NAME}
            sfml-graphics
            sfml-window
            sfml-system
        )
    endif()
else()
    message(FATAL_ERROR "SFML not found!")
endif()

# 设置为控制台子系统（仅 Windows 下适用）
if (MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
endif()
