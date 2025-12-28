cmake_minimum_required(VERSION 3.15)

project(2D_RPG_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # enforce the standard
set(CMAKE_CXX_EXTENSIONS OFF) # disable compiler-specific extensions

# MSVC: 强制标准 __cplusplus 宏
if(MSVC)
    add_compile_options(/Zc:__cplusplus)
endif()

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

add_subdirectory("SFML_test")