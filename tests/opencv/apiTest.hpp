#pragma once
#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>

// #include <opencv4/opencv2/highgui.hpp>
// #include <opencv4/opencv2/imgproc.hpp>


class Test_ :public ::testing::Test
{
    void SetUp() override
    {
        
    }

    void TearDown() override
    {
    }
};

TEST_F(Test_, BaseTest)
{
    using namespace cv;
    std::cout << "Test named: BaseTest\t";
    

    std::cout << "done\n";
}