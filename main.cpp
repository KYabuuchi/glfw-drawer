#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

// プログラムオブジェクトを作成する
// vsrc: バーテックスシェーダのソースプログラムの文字列
// fsrc: フラグメントシェーダのソースプログラムの文字列
GLuint createProgram(const char* vsrc, const char* fsrc)
{
    // 空のプログラムオブジェクトを作成する
    const GLuint program(glCreateProgram());
    if (vsrc != NULL) {
        // バーテックスシェーダのシェーダオブジェクトを作成する
        const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
        glShaderSource(vobj, 1, &vsrc, NULL);
        glCompileShader(vobj);
        // バーテックスシェーダのシェーダオブジェクトをプログラムオブジェクトに組み込む
        glAttachShader(program, vobj);
        glDeleteShader(vobj);
    }
    if (fsrc != NULL) {
        // フラグメントシェーダのシェーダオブジェクトを作成する
        const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
        glShaderSource(fobj, 1, &fsrc, NULL);
        glCompileShader(fobj);
        // フラグメントシェーダのシェーダオブジェクトをプログラムオブジェクトに組み込む
        glAttachShader(program, fobj);
        glDeleteShader(fobj);
    }
    // プログラムオブジェクトをリンクする
    glBindAttribLocation(program, 0, "position");
    glBindFragDataLocation(program, 0, "fragment");
    glLinkProgram(program);
    // 作成したプログラムオブジェクトを返す
    return program;
}

static const char* vsrc = "#version 150 core¥n"
                          "in vec4 position;¥n"
                          "void main()¥n"
                          "{¥n"
                          " gl_Position = position;¥n"
                          "}¥n";

int main()
{
    // GLFW の初期化 (GLFW)
    if (glfwInit() == GL_FALSE) {
        // 初期化に失敗した処理
        std::cerr << "Can't initialize GLFW" << std::endl;
        return 1;
    }

    // 終了時の処理登録 (GLFW)
    std::atexit(glfwTerminate);

    // OpenGL Version 3.2 Core Profile を選択する
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // ウィンドウを作成 (GLFW)
    GLFWwindow* const window(glfwCreateWindow(
        /* int width  = */ 640,
        /* int height  = */ 480,
        /* const char  * title   = */ "Hello!",
        /* GLFWmonitor * monitor = */ NULL,
        /* GLFWwindow  * share   = */ NULL));
    if (window == NULL) {
        // ウィンドウ作成に失敗した処理
        std::cerr << "Can't create GLFW window." << std::endl;
        return 1;
    }

    // 作成したウィンドウを処理対象とする (GLFW)
    glfwMakeContextCurrent(/* GLFWwindow *  window = */ window);

    // 垂直同期のタイミングを待つ
    glfwSwapInterval(1);

    // 背景色 (OpenGL)
    glClearColor(
        /* GLfloat red   = */ 0.2f,
        /* GLfloat green = */ 0.2f,
        /* GLfloat blue  = */ 0.2f,
        /* GLfloat alpha = */ 0.0f);

    // GLEW を初期化する
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        // GLEW の初期化に失敗した
        std::cerr << "Can't initialize GLEW" << std::endl;
        return 1;
    }

    // バーテックスシェーダのソースプログラム
    static constexpr GLchar vsrc[] = "#version 150 core¥n"
                                     "in vec4 position;¥n"
                                     "void main()¥n"
                                     "{¥n"
                                     " gl_Position = position;¥n"
                                     "}¥n";
    // フラグメントシェーダのソースプログラム
    static constexpr GLchar fsrc[] = "#version 150 core¥n"
                                     "out vec4 fragment;¥n"
                                     "void main()¥n"
                                     "{¥n"
                                     " fragment = vec4(1.0, 0.0, 0.0, 1.0);¥n"
                                     "}¥n";
    // プログラムオブジェクトを作成する
    const GLuint program(createProgram(vsrc, fsrc));


    // ウィンドウが開いている間繰り返す
    while (glfwWindowShouldClose(/* GLFWwindow * window = */ window) == GL_FALSE) {
        // ウィンドウを消去 (GLFW)
        glClear(/* GLbitfield mask = */ GL_COLOR_BUFFER_BIT);

        // シェーダプログラムの使用開始
        glUseProgram(program);

        // 描画処理

        // カラーバッファ入れ替え <= ダブルバッファリング (GLFW)
        glfwSwapBuffers(window);

        // イベント待ち (GLFW)
        glfwWaitEvents();
    }

    return 0;
}
