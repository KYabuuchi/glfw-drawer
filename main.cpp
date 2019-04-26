#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <vector>

// シェーダオブジェクトのコンパイル結果を表示する
// shader: シェーダオブジェクト名
// str: コンパイルエラーが発生した場所を示す文字列
GLboolean printShaderInfoLog(GLuint shader, const char* str)
{
    // コンパイル結果を取得する
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
        std::cerr << "Compile Error in " << str << std::endl;
    // シェーダのコンパイル時のログの長さを取得する
    GLsizei bufSize;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);
    if (bufSize > 1) {
        // シェーダのコンパイル時のログの内容を取得する
        std::vector<GLchar> infoLog(bufSize);
        GLsizei length;
        glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
        std::cerr << &infoLog[0] << std::endl;
    }
    return static_cast<GLboolean>(status);
}

// プログラムオブジェクトのリンク結果を表示する
// program: プログラムオブジェクト名
GLboolean printProgramInfoLog(GLuint program)
{
    // リンク結果を取得する
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
        std::cerr << "Link Error." << std::endl;
    // シェーダのリンク時のログの長さを取得する
    GLsizei bufSize;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);
    if (bufSize > 1) {
        // シェーダのリンク時のログの内容を取得する
        std::vector<GLchar> infoLog(bufSize);
        GLsizei length;
        glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
        std::cerr << &infoLog[0] << std::endl;
    }
    return static_cast<GLboolean>(status);
}

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
        if (printShaderInfoLog(vobj, "vertex shader"))
            glAttachShader(program, vobj);
        glDeleteShader(vobj);
    }
    if (fsrc != NULL) {
        // フラグメントシェーダのシェーダオブジェクトを作成する
        const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
        glShaderSource(fobj, 1, &fsrc, NULL);
        glCompileShader(fobj);
        // フラグメントシェーダのシェーダオブジェクトをプログラムオブジェクトに組み込む
        if (printShaderInfoLog(fobj, "fragment shader"))
            glAttachShader(program, fobj);
        glDeleteShader(fobj);
    }
    // プログラムオブジェクトをリンクする
    glBindAttribLocation(program, 0, "position");
    glBindFragDataLocation(program, 0, "fragment");
    glLinkProgram(program);
    // 作成したプログラムオブジェクトを返す
    if (printProgramInfoLog(program))
        return program;

    glDeleteProgram(program);
    return 0;
}

// シェーダのソースファイルを読み込んだメモリを返す
// name: シェーダのソースファイル名
// buffer: 読み込んだソースファイルのテキスト
bool readShaderSource(const char* name, std::vector<GLchar>& buffer)
{
    // ファイル名が NULL だった
    if (name == NULL)
        return false;
    // ソースファイルを開く
    std::ifstream file(name, std::ios::binary);
    if (file.fail()) {
        // 開けなかった
        std::cerr << "Error: Can't open source file: " << name << std::endl;
        return false;
    }
    // ファイルの末尾に移動し現在位置(=ファイルサイズ)を得る
    file.seekg(0L, std::ios::end);
    GLsizei length = static_cast<GLsizei>(file.tellg());
    // ファイルサイズのメモリを確保
    buffer.resize(length + 1);
    // ファイルを先頭から読み込む
    file.seekg(0L, std::ios::beg);
    file.read(buffer.data(), length);
    buffer[length] = '\0';
    if (file.fail()) {
        // うまく読み込めなかった
        std::cerr << "Error: Could not read souce file: " << name << std::endl;
        file.close();
        return false;
    }
    // 読み込み成功
    file.close();
    return true;
}

// シェーダのソースファイルを読み込んでプログラムオブジェクトを作成する
// vert: バーテックスシェーダのソースファイル名
// frag: フラグメントシェーダのソースファイル名
GLuint loadProgram(const char* vert, const char* frag)
{
    // シェーダのソースファイルを読み込む
    std::vector<GLchar> vsrc;
    const bool vstat(readShaderSource(vert, vsrc));
    std::vector<GLchar> fsrc;
    const bool fstat(readShaderSource(frag, fsrc));
    // プログラムオブジェクトを作成する
    return vstat && fstat ? createProgram(vsrc.data(), fsrc.data()) : 0;
}


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
    /// カラーバッファを入れ替えるタイミングを指定する
    glfwSwapInterval(1);

    // 背景色 (OpenGL)
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

    // GLEW を初期化する
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Can't initialize GLEW" << std::endl;
        return 1;
    }

    // プログラムオブジェクトを作成する
    const GLuint program(loadProgram("../point.vert", "../point.frag"));

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

        // イベント待ち(ブロックしない) (GLFW)
        // glfwWaitEvents();
    }

    return 0;
}
