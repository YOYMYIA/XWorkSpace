project(app)

# 只在使用Qt时才需要AUTOMOC
if(Qt5Core_FOUND OR Qt6Core_FOUND)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

# 包含目录 - 添加实际需要的目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJ_SRC_DIR}
    ${THIRD_PARTY_DIR}/${PROJ_3RD}/include
)

# 链接目录 - 添加实际需要的目录
link_directories(
    ${THIRD_PARTY_DIR}/${PROJ_3RD}/lib
    ${INSTALL_PATH_LIB}
)

# 正确设置源文件列表（只包含源文件，不包含头文件）
file(GLOB_RECURSE C_SOURCES "*.c")
file(GLOB_RECURSE CPP_SOURCES "*.cc" "*.cpp")
set(PROJECT_SOURCES ${C_SOURCES} ${CPP_SOURCES})

# 验证源文件列表
if(NOT PROJECT_SOURCES)
    message(FATAL_ERROR "No source files found for project ${PROJECT_NAME}")
endif()

message(STATUS "Source files for ${PROJECT_NAME}:")
foreach(src ${PROJECT_SOURCES})
    message(STATUS "  ${src}")
endforeach()

# 创建可执行文件
if(PROJ_ARCH STREQUAL "x64")
    add_executable(${PROJECT_NAME} WIN32 ${PROJECT_SOURCES})
else()
    add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
endif()

# 添加项目名称作为定义（如果需要）
target_compile_definitions(${PROJECT_NAME} PRIVATE
    PROJECT_NAME="${PROJECT_NAME}"
)

# 设置系统库（确保正确）
if(UNIX)
    set(SYSTEM_LIBS pthread stdc++ m)
elseif(WIN32)
    set(SYSTEM_LIBS ws2_32)
endif()

# 链接库（使用现代CMake语法）
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SYSTEM_LIBS}
    # 添加其他必要的库，如：
    # ${ZMQ_LIBS}
    # ${CURL_LIBS}
)

# 设置输出目录（现代方式）
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJ_BIN_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${PROJ_LIB_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${PROJ_LIB_DIR}
)

# 添加安装目标（可选）
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${PROJ_BIN_DIR}
    LIBRARY DESTINATION ${PROJ_LIB_DIR}
    ARCHIVE DESTINATION ${PROJ_LIB_DIR}
)