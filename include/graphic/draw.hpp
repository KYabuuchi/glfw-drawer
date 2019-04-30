#pragma once
#include <Eigen/Dense>

namespace Graphic
{
void initialize();
void finalize();

// 点群
void drawPoints(const std::vector<Eigen::Vector2d>& points);
// 四角形
void drawRectangle();
// 軌跡
void drawLines();
// 矢印
void drawArrow();
// clear
void clear();

enum Color {
    BLUE = 0,
    GREEN,
    YELLOW,
    WHITE,
};


}  // namespace Graphic