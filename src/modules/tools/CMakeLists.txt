project(Tools)

# 添加导出定义（确保在包含头文件之前定义）
add_definitions(-DTOOLS_API_EXPORTS)

# 收集源文件（排除头文件）
file(GLOB_RECURSE PROJECT_SOURCES "*.c" "*.cc" "*.cpp")

# 验证源文件
if(NOT PROJECT_SOURCES)
    message(FATAL_ERROR "No source files found for project ${PROJECT_NAME}")
endif()

# 包含目录 - 添加实际需要的目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJ_SRC_DIR}  # 假设父CMake定义了PROJ_SRC_DIR
    ${THIRD_PARTY_DIR}/${PROJ_3RD}/include  # 假设父CMake定义了这些变量
)

# 链接目录 - 添加实际需要的目录
link_directories(
    ${THIRD_PARTY_DIR}/${PROJ_3RD}/lib
    ${INSTALL_PATH_LIB}  # 假设父CMake定义了这些变量
)

# 创建共享库
add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})

# 设置目标属性 - 修复错误
set_target_properties(${PROJECT_NAME} PROPERTIES
    # 设置输出目录（现代方式）
    LIBRARY_OUTPUT_DIRECTORY ${PROJ_LIB_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${PROJ_LIB_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${PROJ_BIN_DIR}  # 对于Windows上的DLL
    
    # 可选：设置版本号
    VERSION 1.0.0
    SOVERSION 1
    
    # 可选：设置输出文件名
    OUTPUT_NAME "Tools"
)

# 链接必要的库
target_link_libraries(${PROJECT_NAME} PRIVATE
    # 添加实际需要的库
    # ${SYSTEM_LIBS}
    # ${ZMQ_LIBS}
    # ${CURL_LIBS}
)

# 安装目标（可选）
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${PROJ_LIB_DIR}   # 共享库
    ARCHIVE DESTINATION ${PROJ_LIB_DIR}   # 静态库
    RUNTIME DESTINATION ${PROJ_BIN_DIR}   # Windows上的DLL
)