#include "graphic/draw.hpp"
#include "graphic/graphic.hpp"
#include <chrono>
#include <mutex>
#include <thread>

namespace Graphic
{

namespace
{
std::thread drawing_thread;
std::vector<std::shared_ptr<const Shape>> shape_callback;

const Graphic::Object::Vertex tmp[] = {
    {0.0f, 0.0f, 1.0f, 1.0f, 0.0f},
    {0.0f, 0.5f, 1.0f, 1.0f, 0.0f}};

const Graphic::Object::Vertex rectangleVertex[] = {
    {-0.5f, -0.5f, 1.0f, 1.0f, 0.0f},
    {0.5f, -0.5f, 0.0f, 1.0f, 1.0f},
    {0.5f, 0.5f, 0.0f, 1.0f, 1.0f},
    {-0.5f, 0.5f, 1.0f, 0.0f, 1.0f}};

std::mutex mtx;
std::shared_ptr<const Shape> a;
bool call = false;
bool del = false;

std::shared_ptr<Window> pWindow = nullptr;

}  // namespace


void init()
{
    drawing_thread = std::thread(drawing);
}

void push_back()
{
    pWindow->activate();
    shape_callback.push_back(std::make_shared<const Rectangle>(2, 4, rectangleVertex));
    shape_callback.push_back(std::make_shared<const Points>(2, 1, tmp));
}

void drawRectangle()
{
    std::lock_guard lock(mtx);
    push_back();
    std::cout << "add" << shape_callback.size() << std::endl;
}

void clear()
{
    std::lock_guard lock(mtx);
    shape_callback.clear();
    std::cout << "delete " << shape_callback.size() << std::endl;
}

void join() { drawing_thread.join(); }

void drawing()
{
    // GLFW初期化
    if (glfwInit() == GL_FALSE) {
        std::cerr << "Can't initialize GLFW" << std::endl;
        return;
    }

    std::atexit(glfwTerminate);

    // OpenGL Version 3.2 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    pWindow = std::make_shared<Window>();

    glClearColor(0.3f, 0.2f, 0.2f, 1.0f);

    // GLEW初期化
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Can't initialize GLEW" << std::endl;
        return;
    }
    glViewport(0, 0, 640, 480);

    // プログラムオブジェクトの作成
    const GLuint program = createProgram();
    const GLint sizeLoc(glGetUniformLocation(program, "size"));
    const GLint scaleLoc(glGetUniformLocation(program, "scale"));
    const GLint locationLoc(glGetUniformLocation(program, "location"));


    while (pWindow->shouldClose() == GL_FALSE) {

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glUniform1f(scaleLoc, pWindow->getScale());
        glUniform2fv(sizeLoc, 1, pWindow->getSize());
        glUniform2fv(locationLoc, 1, pWindow->getLocation());

        // 描画
        {
            std::lock_guard lock(mtx);
            for (const std::shared_ptr<const Shape>& e : shape_callback) {
                e->draw();
            }
        }

        pWindow->swapBuffers();

        // 10[ms] wait
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    pWindow = nullptr;
}
}  // namespace Graphic