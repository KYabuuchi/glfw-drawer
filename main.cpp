#include "graphic/load.hpp"
#include "graphic/shape.hpp"
#include "graphic/window.hpp"
#include <memory>

// 矩形の頂点の位置
constexpr Object::Vertex rectangleVertex[] = {
    {-0.5f, -0.5f, 1.0f, 1.0f, 0.0f},
    {0.5f, -0.5f, 0.0f, 1.0f, 1.0f},
    {0.5f, 0.5f, 0.0f, 1.0f, 1.0f},
    {-0.5f, 0.5f, 1.0f, 0.0f, 1.0f}};

int main()
{
    // GLFW の初期化 (GLFW)
    if (glfwInit() == GL_FALSE) {
        std::cerr << "Can't initialize GLFW" << std::endl;
        return 1;
    }

    // 終了時の処理登録 (GLFW)
    std::atexit(glfwTerminate);

    // OpenGL Version 3.2 Core Profile を選択する(GLFW)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // ウィンドウを作成 (GLFW)
    Window window;

    // 背景色 (OpenGL)
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    // GLEW を初期化する
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Can't initialize GLEW" << std::endl;
        return 1;
    }
    // ビューポートを設定する
    glViewport(100, 50, 300, 300);

    // プログラムオブジェクトを作成する
    const GLuint program = createProgram();

    // uniform 変数の場所を取得する
    const GLint sizeLoc(glGetUniformLocation(program, "size"));
    const GLint scaleLoc(glGetUniformLocation(program, "scale"));
    const GLint locationLoc(glGetUniformLocation(program, "location"));

    // 図形データを作成する
    std::unique_ptr<const Shape> shape(new Shape(2, 4, rectangleVertex));

    // ウィンドウが開いている間繰り返す
    while (window.shouldClose() == GL_FALSE) {
        // ウィンドウを消去 (GLFW)
        glClear(/* GLbitfield mask = */ GL_COLOR_BUFFER_BIT);

        // シェーダプログラムの使用開始
        glUseProgram(program);

        // uniform 変数に値を設定する
        glUniform2fv(sizeLoc, 1, window.getSize());
        glUniform1f(scaleLoc, window.getScale());
        glUniform2fv(locationLoc, 1, window.getLocation());

        // 図形を描画する
        shape->draw();

        // カラーバッファ入れ替え <= ダブルバッファリング (GLFW)
        window.swapBuffers();
    }

    return 0;
}
