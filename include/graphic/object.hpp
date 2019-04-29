#pragma once
#include <GL/glew.h>

namespace Graphic
{
class Object
{
    // vertex array object, vertex buffer object
    GLuint vao, vbo;

public:
    struct Vertex {
        GLfloat position[2];
        GLfloat color[3];
    };

    // コンストラクタ
    Object(GLint dimension, GLsizei vertex_count, const Vertex* vertex)
    {
        // 頂点配列オブジェクト
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // 頂点バッファオブジェクト
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(Vertex), vertex, GL_STATIC_DRAW);

        // 結合されている頂点バッファオブジェクトを in 変数から参照できるようにする
        glVertexAttribPointer(0, dimension, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<Vertex*>(0)->position);
        glEnableVertexAttribArray(0);
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
    // 頂点配列オブジェクトの結合
    void bind() const
    {
        // 描画する頂点配列オブジェクトを指定する
        glBindVertexArray(vao);
    }
};
}  // namespace Graphic