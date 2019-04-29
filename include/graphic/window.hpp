#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace Graphic
{
class Window
{
private:
    GLFWwindow* const window;
    GLfloat size[2];
    GLfloat scale;
    GLfloat location[2];

public:
    Window(int width = 640, int height = 480, const char* title = "OpenGL")
        : window(glfwCreateWindow(width, height, title, NULL, NULL)), scale(100.0f), location{0, 0}
    {
        if (window == NULL) {
            std::cerr << "Can't create GLFW window." << std::endl;
            exit(1);
        }
        glfwMakeContextCurrent(window);

        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            std::cerr << "Can't initialize GLEW" << std::endl;
            exit(1);
        }

        // 垂直同期のタイミングを待つ
        glfwSwapInterval(1);

        // このインスタンスの this ポインタを記録
        glfwSetWindowUserPointer(window, this);
        // callback
        glfwSetWindowSizeCallback(window, resize);
        glfwSetScrollCallback(window, wheel);

        resize(window, width, height);
    }

    virtual ~Window() { glfwDestroyWindow(window); }

    // mouse wheel
    static void wheel(GLFWwindow* window, double, double y)
    {
        Window* const
        instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));
        if (instance != NULL) {
            instance->scale += static_cast<GLfloat>(y);
        }
    }
    // ウィンドウのサイズ変更時の処理
    static void resize(GLFWwindow* const window, int width, int height)
    {
        // ウィンドウ全体をビューポートに設定する
        glViewport(0, 0, width, height);

        // このインスタンスの this ポインタを得る
        Window* const instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));
        if (instance != NULL) {
            // 開いたウィンドウのサイズを保存する
            instance->size[0] = static_cast<GLfloat>(width);
            instance->size[1] = static_cast<GLfloat>(height);
        }
    }

    const GLfloat* getSize() const { return size; }
    const GLfloat* getLocation() const { return location; }
    GLfloat getScale() const { return scale; }

    int shouldClose() const { return glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_Q); }

    void swapBuffers()
    {
        glfwSwapBuffers(window);

        // イベント処理待ち
        glfwWaitEvents();
        // glfwPollEvents();

        // マウスの左ボタン
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) != GLFW_RELEASE) {
            double x, y;
            glfwGetCursorPos(window, &x, &y);

            // screen coord => nomrl coord
            location[0] = static_cast<GLfloat>(x) * 2.0f / size[0] - 1.0f;
            location[1] = 1.0f - static_cast<GLfloat>(y) * 2.0f / size[1];
        }
    }
};
}  // namespace Graphic
