cmake_minimum_required(VERSION 3.10)
project(XWorkSpace)

# 检测操作系统平台 - 只允许Linux
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(FATAL_ERROR "This project only supports Linux platform. Detected: ${CMAKE_SYSTEM_NAME}")
endif()

set(PLATFORM_LINUX TRUE)
message(STATUS "Detected Linux platform - proceeding with build")

option(WITH_CMAKE_LEAK "Enable memory leak detection" OFF)
option(WITH_CMAKE_ADDRESS "Enable address sanitizer" OFF)
option(WITH_TEST "Build tests" ON)

# 强制设置为Debug模式
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Architecture handling
if(DEFINED ARCH)
    set(PROJ_ARCH ${ARCH})
else()
    set(PROJ_ARCH "x86")  # Linux默认x86架构
endif()

# Add custom cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Build type configuration - 强制设置为Debug
string(TOUPPER "${CMAKE_BUILD_TYPE}" UPPER_BUILD_TYPE)
if(NOT UPPER_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)  # 默认设置为Debug
    message(STATUS "CMAKE_BUILD_TYPE is not set, defaulting to Debug")
    set(UPPER_BUILD_TYPE "DEBUG")
endif()

# 确保始终使用Debug模式
if(NOT UPPER_BUILD_TYPE STREQUAL "DEBUG")
    message(WARNING "Forcing build type to Debug as requested")
    set(CMAKE_BUILD_TYPE Debug)
    set(UPPER_BUILD_TYPE "DEBUG")
endif()

message(STATUS "Build Type: ${UPPER_BUILD_TYPE}")

# 始终添加调试定义
add_definitions(-DPLAT_DEBUG)

# Linux平台定义
add_definitions(-DPLAT_LINUX)
add_definitions(-D_LARGEFILE_SOURCE)

# 架构特定定义
if(PROJ_ARCH STREQUAL "arm")
    add_definitions(-DPLAT_LINUX_DEVICE)
    set(PROJ_3RD "arm")
elseif(PROJ_ARCH STREQUAL "x86")
    add_definitions(-DMMS_REPLACE_PORT)
    set(PROJ_3RD "x86")
else()
    set(PROJ_3RD "x64")
endif()

# Directory setup
set(PROJ_ROOT_DIR ${PROJECT_SOURCE_DIR})
set(PROJ_SRC_DIR "${PROJ_ROOT_DIR}/src")
set(THIRD_PARTY_DIR "${PROJ_ROOT_DIR}/third_party")

# 设置所有输出到build目录
set(BUILD_OUTPUT_DIR "${PROJECT_BINARY_DIR}")  # build目录本身
set(INSTALL_PATH_LIB "${BUILD_OUTPUT_DIR}/lib")
set(PROJ_BIN_DIR "${BUILD_OUTPUT_DIR}/bin")
set(PROJ_LIB_DIR "${BUILD_OUTPUT_DIR}/lib")

# Third-party libraries configuration - Linux only
set(SYSTEM_LIBS pthread dl m rt) 
 
if(PROJ_ARCH STREQUAL "x64")
    set(ZMQ_LIBS libzmq)
else()
    set(ZMQ_LIBS zmq)
endif()
set(CURL_LIBS "${THIRD_PARTY_DIR}/${PROJ_3RD}/curl/lib/libcurl.a")
set(SSL_LIBS "${THIRD_PARTY_DIR}/${PROJ_3RD}/openssl/lib/libssl.a")
set(SQLITE_LIBS "${THIRD_PARTY_DIR}/${PROJ_3RD}/sqlite/lib/libsqlite3.a")
set(MYSQL_LIBS "${THIRD_PARTY_DIR}/${PROJ_3RD}/mysql/lib/libmysqlclient.a")

# Compiler flags - Linux only
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -g -O0 -Wall -ggdb")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -g -O0 -Wall -ggdb")

# ARM架构特定标志
if(PROJ_ARCH STREQUAL "arm")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-psabi")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-psabi")
endif()

# 覆盖Release标志以确保始终使用Debug
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -O0")

# Sanitizer选项
if(WITH_CMAKE_LEAK)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=leak")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
endif()

if(WITH_CMAKE_ADDRESS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()

# Configuration summary
message(STATUS "************************************")
message(STATUS "Project Configuration Summary:")
message(STATUS "  Operating System: ${CMAKE_SYSTEM_NAME}")
message(STATUS "  Project Root: ${PROJ_ROOT_DIR}")
message(STATUS "  Architecture: ${PROJ_ARCH}")
message(STATUS "  3rd Party Dir: ${PROJ_3RD}")
message(STATUS "  Binary Output: ${PROJ_BIN_DIR}")
message(STATUS "  Library Output: ${PROJ_LIB_DIR}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE} (forced)")
message(STATUS "************************************")

# )# Include directories
include_directories(
    ${PROJ_SRC_DIR}
    ${THIRD_PARTY_DIR}/${PROJ_3RD}/include
)

# Link directories
link_directories(
    ${THIRD_PARTY_DIR}/${PROJ_3RD}/lib
    ${PROJ_LIB_DIR}  # 添加build/lib到链接路径
)

# 设置全局输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJ_LIB_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJ_LIB_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJ_BIN_DIR})

# 创建输出目录
file(MAKE_DIRECTORY ${PROJ_BIN_DIR})
file(MAKE_DIRECTORY ${PROJ_LIB_DIR})

# 添加子项目
add_subdirectory(src)
add_subdirectory(tests)