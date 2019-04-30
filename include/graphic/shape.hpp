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
    virtual void execute() const = 0;

public:
    Shape(GLint size, GLsizei vertex_count, const Object::Vertex* vertices)
        : object(std::make_shared<const Object>(size, vertex_count, vertices)),
          vertex_count(vertex_count) {}

    void draw() const
    {
        object->bind();
        execute();
    }
};

class Triangle : public Shape
{
protected:
    virtual void execute() const override { glDrawArrays(GL_LINE_LOOP, 0, vertex_count); }

public:
    Triangle(GLint size, GLsizei vertex_count, const Object::Vertex* vertices)
        : Shape(size, vertex_count, vertices) {}
};

class Rectangle : public Shape
{
protected:
    virtual void execute() const override { glDrawArrays(GL_LINE_LOOP, 0, vertex_count); }

public:
    Rectangle(GLint size, GLsizei vertex_count, const Object::Vertex* vertices)
        : Shape(size, vertex_count, vertices) {}
};

class Points : public Shape
{
protected:
    virtual void execute() const override { glDrawArrays(GL_POINTS, 0, vertex_count); }

public:
    Points(GLint size, GLsizei vertex_count, const Object::Vertex* vertices)
        : Shape(size, vertex_count, vertices) {}
};

}  // namespace Graphic