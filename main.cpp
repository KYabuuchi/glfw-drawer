#include "graphic/draw.hpp"
#include <chrono>
#include <iostream>
#include <thread>

int main()
{
    using namespace Graphic;
    initialize();

    std::vector<Eigen::Vector2d> points1;
    std::vector<Eigen::Vector2d> points2;
    for (int i = 0; i < 10; i++) {
        points1.push_back(Eigen::Vector2d::Random());
        points2.push_back(Eigen::Vector2d::Random());
    }


    while (1) {
        if (not isRunning())
            break;
        drawArrow(points1[0], points1[1], Color::YELLOW);
        drawArrow(points1[2], points1[3], Color::YELLOW);
        drawArrow(points1[4], points1[5], Color::YELLOW);
        drawArrow(points1[6], points1[7], Color::YELLOW);
        draw(points2, Form::POINTS, Color::RED);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        clear();
        draw(Eigen::Vector2d(0.5, 0.5), Form::POINT, Color::GREEN);
        draw(points1, Form::CURVE, Color::PURPLE);
        draw(points2, Form::RECTANGLE, Color::PURPLE);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        clear();
    }

    finalize();
}
