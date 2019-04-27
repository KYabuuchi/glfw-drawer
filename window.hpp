#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
// ウィンドウ関連の処理
class Window
{
    // ウィンドウのハンドル
    GLFWwindow* const window;
    // ウィンドウのサイズ
    GLfloat size[2];
    // ワールド座標系に対するデバイス座標系の拡大率
    GLfloat scale;

public:
    // コンストラクタ
    Window(int width = 640, int height = 480, const char* title = "Hello!")
        : window(glfwCreateWindow(width, height, title, NULL, NULL)), scale(100.0f)
    {
        if (window == NULL) {
            // ウィンドウが作成できなかった
            std::cerr << "Can't create GLFW window." << std::endl;
            exit(1);
        }
        // 現在のウィンドウを処理対象にする
        glfwMakeContextCurrent(window);
        // GLEW を初期化する
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            // GLEW の初期化に失敗した
            std::cerr << "Can't initialize GLEW" << std::endl;
            exit(1);
        }
        // 垂直同期のタイミングを待つ
        glfwSwapInterval(1);
        // このインスタンスの this ポインタを記録しておく
        glfwSetWindowUserPointer(window, this);
        // ウィンドウのサイズ変更時に呼び出す処理の登録
        glfwSetWindowSizeCallback(window, resize);
        // 開いたウィンドウの初期設定
        resize(window, width, height);
    }
    // デストラクタ
    virtual ~Window()
    {
        glfwDestroyWindow(window);
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
    // ウィンドウのサイズを取り出す
    const GLfloat* getSize() const { return size; }
    // ワールド座標系に対するデバイス座標系の拡大率を取り出す
    GLfloat getScale() const { return scale; }

    // ウィンドウを閉じるべきかを判定する
    int shouldClose() const
    {
        return glfwWindowShouldClose(window);
    }
    // カラーバッファを入れ替えてイベントを取り出す
    void swapBuffers()
    {
        // カラーバッファを入れ替える
        glfwSwapBuffers(window);
        // イベントを取り出す
        glfwWaitEvents();
    }
};