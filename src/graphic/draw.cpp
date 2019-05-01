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
std::shared_ptr<Window> window = nullptr;

std::mutex draw_mtx;
std::mutex stop_mtx;
bool stop_request = false;

const std::vector<std::array<GLfloat, 3>> RGB
    = {
        {1.0f, 0.3f, 0.3f},
        {0.3f, 1.0f, 0.3f},
        {0.3f, 0.3f, 1.0f},
        {1.0f, 1.0f, 0.2f},
        {0.9f, 0.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
        {0.0f, 0.0f, 0.0f},
};

}  // namespace

void drawing()
{
    window->activate();

    const GLuint program = createProgram();
    const GLint sizeLoc(glGetUniformLocation(program, "size"));
    const GLint scaleLoc(glGetUniformLocation(program, "scale"));
    const GLint locationLoc(glGetUniformLocation(program, "location"));

    while (window->shouldClose() == GL_FALSE) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glUniform1f(scaleLoc, window->getScale());
        glUniform2fv(sizeLoc, 1, window->getSize());
        glUniform2fv(locationLoc, 1, window->getLocation());

        // 描画
        {
            std::lock_guard lock(draw_mtx);
            for (const std::shared_ptr<const Shape>& e : shape_callback) {
                e->draw();
            }
        }
        window->swapBuffers();

        // 10[ms] wait
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        // check stop request
        {
            std::lock_guard lock(draw_mtx);
            if (stop_request)
                break;
        }
    }

    window = nullptr;
}

bool isRunning()
{
    return window != nullptr;
}

void initialize()
{
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

    window = std::make_shared<Window>(640, 480, "OpenGL");
    glViewport(0, 0, 640, 480);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Can't initialize GLEW" << std::endl;
        return;
    }

    // start thread
    drawing_thread = std::thread(drawing);
}

void finalize()
{
    {
        std::lock_guard lock(draw_mtx);
        stop_request = true;
    }
    drawing_thread.join();
}

void draw(const Eigen::Vector2d& point, Form form, Color color)
{
    draw(std::vector<Eigen::Vector2d>{point}, form, color);
}

void draw(const std::vector<Eigen::Vector2d>& points, Form form, Color color)
{
    std::vector<Object::Vertex> tmp;
    for (const auto& p : points) {
        tmp.push_back({static_cast<GLfloat>(p.x()),
            static_cast<GLfloat>(p.y()),
            RGB[color][0], RGB[color][1], RGB[color][2]});
    }

    std::lock_guard lock(draw_mtx);
    shape_callback.push_back(std::make_shared<const Shape>(tmp, form));
}

void drawArrow(const Eigen::Vector2d& head, const Eigen::Vector2d& tail, Color color)
{
    std::vector<Object::Vertex> tmp;
    double k = 0.2;
    Eigen::Vector2d d = head - tail;
    Eigen::Vector2d n(-d.y(), d.x());
    n = n / n.norm() * d.norm();
    Eigen::Vector2d neck1 = head - d * 0.3 + k * n;
    Eigen::Vector2d neck2 = head - d * 0.3 - k * n;

    std::vector<Eigen::Vector2d> points = {head, neck1, neck2, head, tail};
    for (const auto& p : points)
        tmp.push_back({static_cast<GLfloat>(p.x()), static_cast<GLfloat>(p.y()),
            RGB[color][0], RGB[color][1], RGB[color][2]});

    std::lock_guard lock(draw_mtx);
    shape_callback.push_back(std::make_shared<const Shape>(tmp, Form::CURVE));
}

void drawArrow(const std::vector<Eigen::Vector2d>& points, Color color)
{
    drawArrow(points[0], points[1], color);
}


void clear()
{
    std::lock_guard lock(draw_mtx);
    shape_callback.clear();
}

}  // namespace Graphic