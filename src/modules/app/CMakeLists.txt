project(app)

# 只在使用Qt时才需要AUTOMOC - 保留但添加Linux条件
if((Qt5Core_FOUND OR Qt6Core_FOUND) AND PLATFORM_LINUX)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(glfw3 REQUIRED)
# 关键修改1：添加template库的包含路径
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJ_SRC_DIR}
    ${PROJ_SRC_DIR}/modules
    ${PROJ_SRC_DIR}/modules/template
    ${PROJ_SRC_DIR}/modules/tools
    ${THIRD_PARTY_DIR}/${PROJ_3RD}/include
)

# 链接目录 - 仅Linux路径
link_directories(
    ${THIRD_PARTY_DIR}/${PROJ_3RD}/lib
    ${PROJ_LIB_DIR}
)

# 正确设置源文件列表
file(GLOB_RECURSE C_SOURCES "*.c")
file(GLOB_RECURSE CPP_SOURCES "*.cc" "*.cpp")
set(PROJECT_SOURCES ${C_SOURCES} ${CPP_SOURCES})

# 验证源文件列表
if(NOT PROJECT_SOURCES)
    message(FATAL_ERROR "No source files found for project ${PROJECT_NAME}")
endif()

# 创建可执行文件 - 仅Linux格式
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# 添加项目名称作为定义
target_compile_definitions(${PROJECT_NAME} PRIVATE
    PROJECT_NAME="${PROJECT_NAME}"
)

# 设置系统库 - 仅Linux
set(SYSTEM_LIBS pthread stdc++ m)

# 关键修改2：链接template库
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SYSTEM_LIBS}
    Template  # 确保链接template库
    Tools
    glfw
)

# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJ_BIN_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${PROJ_LIB_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${PROJ_LIB_DIR}
)

# 安装目标
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${PROJ_BIN_DIR}
    LIBRARY DESTINATION ${PROJ_LIB_DIR}
    ARCHIVE DESTINATION ${PROJ_LIB_DIR}
)

# 关键修改3：确保template库已构建
if(TARGET Template)
    add_dependencies(${PROJECT_NAME} Template)
endif()