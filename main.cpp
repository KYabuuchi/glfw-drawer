#include "graphic/draw.hpp"
#include <chrono>
#include <iostream>
#include <thread>

int main()
{
    using namespace Graphic;
    initialize();

    std::vector<Eigen::Vector2d> points;
    for (int i = 0; i < 10; i++) {
        points.push_back(Eigen::Vector2d::Random());
    }


    while (1) {
        if (not isRunning())
            break;

        draw(points, Form::POINTS, Color::YELLOW);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        clear();
        draw(Eigen::Vector2d(0.5, 0.5), Form::POINT, Color::GREEN);
        draw(points, Form::CURVE, Color::PURPLE);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        clear();
    }

    finalize();
}
