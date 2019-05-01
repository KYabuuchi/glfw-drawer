#pragma once
#include "graphic/shape.hpp"
#include <Eigen/Dense>

namespace Graphic
{
void initialize();
void finalize();
bool isRunning();

enum Color {
    RED = 0,
    GREEN,
    BLUE,
    YELLOW,
    PURPLE,
    WHITE,
    BLACK,
};

void draw(const Eigen::Vector2d& point, Form form = Form::POINT, Color color = Color::WHITE);
void draw(const std::vector<Eigen::Vector2d>& points, Form form, Color color = Color::WHITE);
void drawArrow(const Eigen::Vector2d& head, const Eigen::Vector2d& tail, Color color = Color::WHITE);
void drawArrow(const std::vector<Eigen::Vector2d>& points, Color color = Color::WHITE);
void clear();


}  // namespace Graphic