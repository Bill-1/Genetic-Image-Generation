#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <array>
#include <cstdint>
#include <config.h>

class Image;

struct Triangle {
    std::array<int, 2> v1;
    std::array<int, 2> v2;
    std::array<int, 2> v3;
    int color;
    bool active;

    Triangle(int h, int w);
    Triangle(std::array<int, 2> &p1, std::array<int, 2> &p2, std::array<int, 2> &p3);

    void mutate(const Image &target, Image &im, double &loss);

    bool isIn(std::array<int, 2> &point);

    int getArea();
};

#endif 