# tests/CMakeLists.txt

if(WITH_TEST AND PLATFORM_LINUX)
    message(STATUS "Configuring tests for Linux platform")
    
    find_package(GTest REQUIRED)
    include(GoogleTest)
    
    set(TEST_BIN_DIR "${PROJ_BIN_DIR}")
    file(MAKE_DIRECTORY ${TEST_BIN_DIR})
    
    # 打印关键路径用于调试
    message(STATUS "PROJ_ROOT_DIR = ${PROJ_ROOT_DIR}")
    message(STATUS "THIRD_PARTY_DIR = ${THIRD_PARTY_DIR}")
    
    file(GLOB TEST_SUBDIRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*")
    
    foreach(TEST_DIR ${TEST_SUBDIRS})
        if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_DIR}")
            message(STATUS "Found test directory: ${TEST_DIR}")
            
            file(GLOB TEST_SOURCES 
                "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_DIR}/*.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_DIR}/*.cc"
                "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_DIR}/*.c"
            )
            
            if(TEST_SOURCES)
                string(REPLACE "/" "_" TEST_TARGET_NAME "${TEST_DIR}")
                set(TEST_TARGET "test_${TEST_TARGET_NAME}")
                
                message(STATUS "  Adding test target: ${TEST_TARGET}")
                
                add_executable(${TEST_TARGET} ${TEST_SOURCES})
                
                set_target_properties(${TEST_TARGET} PROPERTIES
                    RUNTIME_OUTPUT_DIRECTORY ${TEST_BIN_DIR}
                )
                
                # 关键修复：包含主项目的src目录
                target_include_directories(${TEST_TARGET} PRIVATE
                    ${PROJ_ROOT_DIR}/src/modules                  # 主项目头文件
                    ${THIRD_PARTY_DIR}/${PROJ_3RD}/include  # 第三方头文件
                    ${GTEST_INCLUDE_DIRS}                  # GTest头文件
                )
                
                # 打印包含路径用于调试
                get_target_property(INCL_DIRS ${TEST_TARGET} INCLUDE_DIRECTORIES)
                message(STATUS "  Include directories for ${TEST_TARGET}:")
                foreach(DIR ${INCL_DIRS})
                    message(STATUS "    - ${DIR}")
                endforeach()
                
                target_link_libraries(${TEST_TARGET} PRIVATE
                    GTest::gtest
                    GTest::gtest_main
                    ${SYSTEM_LIBS}
                )
                
                # 链接主项目库（如果存在）
                if(TARGET XWorkSpace_lib)  # 替换为实际库名
                    message(STATUS "  Linking with main project library")
                    target_link_libraries(${TEST_TARGET} PRIVATE XWorkSpace_lib)
                endif()
                
                gtest_discover_tests(${TEST_TARGET}
                    WORKING_DIRECTORY ${TEST_BIN_DIR}
                )
                
                # ... [其他设置保持不变] ...
                # 添加调试标志
                target_compile_options(${TEST_TARGET} PRIVATE
                    -g -O0 -ggdb -fPIC
                )
                
                # ARM架构特定标志
                if(PROJ_ARCH STREQUAL "arm")
                    target_compile_options(${TEST_TARGET} PRIVATE
                        -Wno-psabi
                    )
                endif()
                
            endif()
        endif()
    endforeach()
endif()