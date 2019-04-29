#pragma once
#include "graphic/object.hpp"
#include <memory>

namespace Graphic
{
class Shape
{
private:
    std::shared_ptr<const Object> object;

protected:
    const GLsizei vertex_count;

public:
    Shape(GLint size, GLsizei vertex_count, const Object::Vertex* vertex)
        : object(new Object(size, vertex_count, vertex)), vertex_count(vertex_count) {}

    void draw() const
    {
        object->bind();
        execute();
    }

    virtual void execute() const
    {
        // 折れ線
        glDrawArrays(GL_LINE_LOOP, 0, vertex_count);
    }
};
}  // namespace Graphic