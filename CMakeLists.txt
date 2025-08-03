cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    add_compile_options(/Zc:__cplusplus)
endif()

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("CMakeProject1")

# 包含子项目。
add_subdirectory("SFML_test")