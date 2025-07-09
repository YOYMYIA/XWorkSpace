#include "tools/DebugLog.h"
#include <gtest/gtest.h>
#include "test.hpp"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    DBG_LOGI("Starting unit tests...");
    return RUN_ALL_TESTS();
}