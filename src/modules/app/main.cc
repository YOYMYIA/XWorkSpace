#include <glad/glad.h>  // 必须放在GLFW之前
#include <GLFW/glfw3.h>
#include <iostream>      // 用于错误输出
#include "tools/DebugLog.h"

// 错误回调函数
void error_callback(int error, const char* description) {
    std::cerr << "Error: " << description << std::endl;
}

int main(void) {
    GLFWwindow* window;
    DBG_LOGI("Hello World");

    // 设置错误回调
    glfwSetErrorCallback(error_callback);

    /* 初始化glfw */
    if (!glfwInit())
        return -1;

    // 设置OpenGL版本 (重要!)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // macOS需要
    #endif

    /* 创建一个Window 和 OpenGL上下文 */
    window = glfwCreateWindow(960, 640, "Hello World", NULL, NULL);
    if (!window) {
        // 创建失败就退出
        glfwTerminate();
        return -1;
    }

    /* 激活上面创建的OpenGL上下文 */
    glfwMakeContextCurrent(window);
    
    // ===== 关键修复 ===== //
    // 初始化GLAD (取消注释这行)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // =================== //
    
    // 设置视口
    glViewport(0, 0, 960, 640);

    /* 进入游戏引擎主循环 */
    while (!glfwWindowShouldClose(window)) {
        /* 处理鼠标键盘事件 */
        glfwPollEvents();
        
        // 检查ESC键是否按下
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        /* 渲染代码 */
        glClearColor(49.f/255, 77.f/255, 121.f/255, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* 交换前后缓冲区 */
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}