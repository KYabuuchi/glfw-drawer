#pragma once
#include "graphic/draw.hpp"
#include "graphic/object.hpp"
#include <memory>

namespace Graphic
{
enum Form {
    POINT = 0,
    POINTS,
    CURVE,
    RECTANGLE,
};

class Shape
{
private:
    std::shared_ptr<const Object> object;

protected:
    Form form;
    int vertex_size;

    virtual void execute() const
    {
        switch (form) {
        case POINT:
            glDrawArrays(GL_POINTS, 0, vertex_size);
            break;
        case POINTS:
            glDrawArrays(GL_POINTS, 0, vertex_size);
            break;
        case CURVE:
            glDrawArrays(GL_LINE_STRIP, 0, vertex_size);
            break;
        case RECTANGLE:
            glDrawArrays(GL_LINE_LOOP, 0, 4);
            break;
        default:
            glDrawArrays(GL_POINTS, 0, vertex_size);
            break;
        }
    }

public:
    Shape(const std::vector<Object::Vertex>& vertices, Form form, GLint dimension = 2)
        : object(std::make_shared<const Object>(vertices, static_cast<int>(vertices.size()), dimension)),
          form(form),
          vertex_size(static_cast<int>(vertices.size()))
    {
    }

    void draw() const
    {
        object->bind();
        execute();
    }
};


// class Rectangle : public Shape
// {
// protected:
//     virtual void execute() const override { glDrawArrays(GL_LINE_LOOP, 0, vertex_count); }

// public:
//     Rectangle(GLint size, GLsizei vertex_count, const std::vector<Object::Vertex>& vertices)
//         : Shape(size, vertex_count, vertices) {}
// };

// class Points : public Shape
// {
// protected:
//     virtual void execute() const override { glDrawArrays(GL_POINTS, 0, vertex_count); }

// public:
//     Points(GLint size, GLsizei vertex_count, const std::vector<Object::Vertex>& vertices)
//         : Shape(size, vertex_count, vertices) {}
// };

// class Curve : public Shape
// {
// protected:
//     virtual void execute() const override { glDrawArrays(GL_LINE_STRIP, 0, vertex_count); }

// public:
//     Curve(GLint size, GLsizei vertex_count, const std::vector<Object::Vertex>& vertices)
//         : Shape(size, vertex_count, vertices) {}
// };

}  // namespace Graphic