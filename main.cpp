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


    for (int i = 0; i < 3; i++) {
        drawPoints(points);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        clear();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    finalize();
}
