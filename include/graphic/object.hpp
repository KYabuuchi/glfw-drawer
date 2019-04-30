#pragma once
#include <GL/glew.h>

namespace Graphic
{
class Object
{
    GLuint vao, vbo;

public:
    struct Vertex {
        GLfloat position[2];
        GLfloat color[3];
    };

    // コンストラクタ
    Object(GLint dimension, GLsizei vertex_count, const Vertex* vertex)
    {
        // VAO
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // VBO
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(Vertex), vertex, GL_STATIC_DRAW);

        // attribute: position
        glVertexAttribPointer(0, dimension, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<Vertex*>(0)->position);
        glEnableVertexAttribArray(0);

        // attribute: color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<Vertex*>(0)->color);
        glEnableVertexAttribArray(1);
    };

    virtual ~Object()
    {
        glDeleteBuffers(1, &vao);
        glDeleteBuffers(1, &vbo);
    }

private:
    // コピーの禁止
    Object(const Object& o);
    Object& operator=(const Object& o);

public:
    void bind() const
    {
        glBindVertexArray(vao);
    }
};
}  // namespace Graphic