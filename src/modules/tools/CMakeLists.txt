project(Tools)

# 添加导出定义
add_definitions(-DTOOLS_API_EXPORTS)

# 收集源文件
file(GLOB_RECURSE PROJECT_SOURCES 
    "*.c"
    "*.cc"
    "*.cpp"
)

# 收集头文件（用于安装）
file(GLOB_RECURSE PROJECT_HEADERS 
    "*.h"
    "*.hpp"
    "*.inl"
)

if(NOT PROJECT_SOURCES)
    message(FATAL_ERROR "No source files found for project ${PROJECT_NAME}")
endif()

# 创建共享库
add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})

# 关键：设置目标包含目录
target_include_directories(${PROJECT_NAME}
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>  # 构建时：父目录
        $<INSTALL_INTERFACE:include>                       # 安装时：include目录
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJ_SRC_DIR}
        ${THIRD_PARTY_DIR}/${PROJ_3RD}/include
)

# 设置目标属性（仅Linux）
set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PROJ_LIB_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${PROJ_LIB_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${PROJ_BIN_DIR}
    VERSION 1.0.0
    SOVERSION 1
    OUTPUT_NAME "Tools"
    # Linux特定设置
    POSITION_INDEPENDENT_CODE ON
)

# 链接库（仅Linux系统库）
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SYSTEM_LIBS}  # Linux系统库 (pthread, stdc++, m等)
)

# 安装目标（仅Linux路径）
install(TARGETS ${PROJECT_NAME}
    EXPORT ToolsTargets
    LIBRARY DESTINATION ${PROJ_LIB_DIR}
    ARCHIVE DESTINATION ${PROJ_LIB_DIR}
    RUNTIME DESTINATION ${PROJ_BIN_DIR}
)

# 安装头文件（保持目录结构）
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include/Tools
    FILES_MATCHING 
        PATTERN "*.h"
        PATTERN "*.hpp"
        PATTERN "*.inl"
)

# 导出目标（可选，便于其他项目使用）
install(EXPORT ToolsTargets
    FILE ToolsConfig.cmake
    DESTINATION ${PROJ_LIB_DIR}/cmake/Tools
)