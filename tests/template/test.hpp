//
// Created by xlinux on 25-7-9.
//

#ifndef __TEST_HPP__
#define __TEST_HPP__
#include "template/test.hpp"
#include <gtest/gtest.h>

using namespace test;

class Test_ :public ::testing::Test
{
    void SetUp() override
    {
        RemoveReferenceConst<const int&> h = 3;
        std::cout << "h:" << h << std::endl;
        EXPECT_EQ(h, 3);
    }

    void TearDown() override
    {

    }
};

TEST_F(Test_, OneTest)
{
    RemoveReferenceConst<const int&> h = 3;
    std::cout << "h:" << h << std::endl;
    EXPECT_EQ(h, 3);

}

#endif //TEST_HPP
