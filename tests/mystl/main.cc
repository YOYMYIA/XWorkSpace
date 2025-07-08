#include "tools/DebugLog.h"
#include "binaryTree.hpp"
#include "template/mystl/until.hpp"

#include <gtest/gtest.h>

int main(int argc, char **argv) {
    // 初始化Google Test
    ::testing::InitGoogleTest(&argc, argv);
    
    // 设置日志级别为INFO
    DBG_LOGI("Starting unit tests...");

    // 运行所有测试
    int result = RUN_ALL_TESTS();

    // 输出测试结果
    DBG_LOGI("Unit tests completed with result: %d", result);

    return result;
}   
