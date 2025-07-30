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
     // 1. 创建测试矩阵并进行基本运算
    cv::Mat A = (cv::Mat_<double>(3, 3) << 1, 2, 3, 4, 5, 6, 7, 8, 9);
    cv::Mat B = (cv::Mat_<double>(3, 3) << 9, 8, 7, 6, 5, 4, 3, 2, 1);
    
    // 矩阵加法
    cv::Mat C = A + B;
    
    // 矩阵乘法
    cv::Mat D = A * B;
    
    // 矩阵转置
    cv::Mat E = A.t();
    
    // 矩阵求逆 (使用伪逆避免奇异矩阵问题)
    cv::Mat F = (A + cv::Mat::eye(A.size(), A.type())).inv();
    
    // 2. 特征值和特征向量计算
    cv::Mat eigenvalues, eigenvectors;
    cv::eigen(A, eigenvalues, eigenvectors);
    
    // 3. 奇异值分解 (SVD)
    cv::Mat S, U, Vt;
    cv::SVDecomp(A, S, U, Vt);
    
    // 4. 统计计算
    cv::Scalar mean, stddev;
    cv::meanStdDev(A, mean, stddev);
    
    // 5. 几何变换矩阵
    cv::Mat rotMatrix = cv::getRotationMatrix2D(cv::Point2f(0, 0), 45, 1.0);
    
    // 输出结果
    std::cout << "=== 矩阵运算测试 ===\n";
    std::cout << "矩阵 A:\n" << A << "\n\n";
    std::cout << "矩阵 B:\n" << B << "\n\n";
    std::cout << "A + B:\n" << C << "\n\n";
    std::cout << "A * B:\n" << D << "\n\n";
    std::cout << "A的转置:\n" << E << "\n\n";
    std::cout << "(A+I)的伪逆:\n" << F << "\n\n";
    
    std::cout << "=== 特征值分解 ===\n";
    std::cout << "特征值:\n" << eigenvalues << "\n";
    std::cout << "特征向量:\n" << eigenvectors << "\n\n";
    
    std::cout << "=== 奇异值分解 ===\n";
    std::cout << "奇异值:\n" << S << "\n";
    std::cout << "U矩阵:\n" << U << "\n";
    std::cout << "Vt矩阵:\n" << Vt << "\n\n";
    
    std::cout << "=== 统计信息 ===\n";
    std::cout << "均值: " << mean[0] << "\n";
    std::cout << "标准差: " << stddev[0] << "\n\n";
    
    std::cout << "=== 几何变换 ===\n";
    std::cout << "45度旋转矩阵:\n" << rotMatrix << "\n";
    
    // 6. 验证OpenCV版本
    std::cout << "\n=== 系统验证 ===\n";
    std::cout << "OpenCV版本: " << CV_VERSION << "\n";
    std::cout << "构建信息: " << cv::getBuildInformation() << "\n";
    
}