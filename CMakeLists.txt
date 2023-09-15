# CMakeList.txt: testctp 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("testctp") 
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
SET(SRC ${CMAKE_CURRENT_SOURCE_DIR}/testctp.cpp ${CMAKE_CURRENT_SOURCE_DIR}/MymdSpi.cpp ${CMAKE_CURRENT_SOURCE_DIR}/MyTrSpi.cpp)
link_directories(${CMAKE_CURRENT_SOURCE_DIR})
# 将源代码添加到此项目的可执行文件。  
add_executable (testctp ${SRC} "MyTrSpi.h" "MyTrSpi.cpp")
target_link_libraries(testctp thostmduserapi_se thosttraderapi_se)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET testctp PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。
