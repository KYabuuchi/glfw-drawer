#include "graphic/graphic.hpp"
#include <memory>

constexpr Graphic::Object::Vertex tmp[] = {
    {0.0f, 0.0f, 1.0f, 1.0f, 0.0f},
    {0.0f, 0.5f, 1.0f, 1.0f, 0.0f}};
constexpr Graphic::Object::Vertex rectangleVertex[] = {
    {-0.5f, -0.5f, 1.0f, 1.0f, 0.0f},
    {0.5f, -0.5f, 0.0f, 1.0f, 1.0f},
    {0.5f, 0.5f, 0.0f, 1.0f, 1.0f},
    {-0.5f, 0.5f, 1.0f, 0.0f, 1.0f}};

int main()
{
    using namespace Graphic;

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
    Window window1(640, 480, "2");

    // 背景色
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // GLEW を初期化する
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Can't initialize GLEW" << std::endl;
        return 1;
    }
    // ビューポートを設定する
    glViewport(0, 0, 640, 480);

    // プログラムオブジェクトを作成する
    const GLuint program = createProgram();

    // uniform 変数の場所を取得する
    const GLint sizeLoc(glGetUniformLocation(program, "size"));
    const GLint scaleLoc(glGetUniformLocation(program, "scale"));
    const GLint locationLoc(glGetUniformLocation(program, "location"));

    std::unique_ptr<const Shape> shape1 = std::make_unique<const Rectangle>(2, 4, rectangleVertex);
    std::unique_ptr<const Shape> shape2 = std::make_unique<const Points>(2, 1, tmp);

    while (window.shouldClose() == GL_FALSE) {
        glClear(/* GLbitfield mask = */ GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        // uniform 変数に値を設定する
        glUniform1f(scaleLoc, window.getScale());
        glUniform2fv(sizeLoc, 1, window.getSize());
        glUniform2fv(locationLoc, 1, window.getLocation());

        // uniform 変数に値を設定する
        glUniform1f(scaleLoc, window1.getScale());
        glUniform2fv(sizeLoc, 1, window1.getSize());
        glUniform2fv(locationLoc, 1, window1.getLocation());


        // 図形を描画する
        shape1->draw();
        shape2->draw();

        window.swapBuffers();
        window1.swapBuffers();
    }

    return 0;
}
