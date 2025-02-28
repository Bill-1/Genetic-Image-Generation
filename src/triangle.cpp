#include "image.h"
#include <iostream>

Triangle::Triangle() : v1{{0, 0}}, v2{{0, 0}}, v3{{0, 0}}, color(0) {} 

inline int clamp(int value, int minVal, int maxVal) {
    return std::max(minVal, std::min(value, maxVal));
}

Triangle::Triangle(std::mt19937 &gen, int H, int W, double rate) {
    int h = H * rate;
    int w = W * rate;
    
    std::uniform_int_distribution<>disH(0, H - 1);
    std::uniform_int_distribution<>disW(0, W - 1);
    std::uniform_int_distribution<>displace(-50, 50);
    int centerX = disH(gen);
    int centerY = disW(gen); 
    v1[0] = clamp(centerX + displace(gen), 0, H - 1);
    v2[0] = clamp(centerX + displace(gen), 0, H - 1);
    v3[0] = clamp(centerX + displace(gen), 0, H - 1);
    v1[1] = clamp(centerY + displace(gen), 0, W - 1);
    v2[1] = clamp(centerY + displace(gen), 0, W - 1);
    v3[1] = clamp(centerY + displace(gen), 0, W - 1);
    
    std::uniform_int_distribution<>colorDis(0, 255);
    int red = colorDis(gen);
    int green = colorDis(gen);
    int blue = colorDis(gen);
    int alpha = colorDis(gen);
    color = (red << 16) | (green << 8) | blue;
}

Triangle::Triangle(std::array<int, 2> &p1, std::array<int, 2> &p2, std::array<int, 2> &p3) : v1(p1), v2(p2), v3(p3), color(0) {}

void Triangle::mutate(std::mt19937 &gen) {
    double chance = dis(gen);
    if (chance <= MUTATION_RATES[0]) {
        std::uniform_int_distribution<> displace(-50, 50);
        int x = displace(gen);
        int y = displace(gen);
        v1[0] = clamp(v1[0] + x, 0, HEIGHT - 1);
        v1[1] = clamp(v1[1] + y, 0, WIDTH - 1);
        v2[0] = clamp(v2[0] + x, 0, HEIGHT - 1);
        v2[1] = clamp(v2[1] + y, 0, WIDTH - 1);
        v3[0] = clamp(v3[0] + x, 0, HEIGHT - 1);
        v3[1] = clamp(v3[1] + y, 0, WIDTH - 1);
    } else if (chance <= MUTATION_RATES[1]) {
        std::uniform_int_distribution<> displace(-50, 50);
        int id = rand() % 3;
        if (id == 0) {
            v1[0] = clamp(v1[0] + displace(gen), 0, HEIGHT - 1);
            v1[1] = clamp(v1[1] + displace(gen), 0, WIDTH - 1);
        } else if (id == 1) {
            v2[0] = clamp(v2[0] + displace(gen), 0, HEIGHT - 1);
            v2[1] = clamp(v2[1] + displace(gen), 0, WIDTH - 1);
        } else {
            v3[0] = clamp(v3[0] + displace(gen), 0, HEIGHT - 1);
            v3[1] = clamp(v3[1] + displace(gen), 0, WIDTH - 1);
        }
    } else if (chance <= MUTATION_RATES[2]) {
        std::uniform_int_distribution<> displace(-50, 50);
        int alpha = color >> 24;
        int red = (color >> 16) & 0xFF;
        int green = (color >> 8) & 0xFF;    
        int blue = color & 0xFF;
        red = clamp(red + displace(gen), 0, 255);
        green = clamp(green + displace(gen), 0, 255);
        blue = clamp(blue + displace(gen), 0, 255);
        alpha = clamp(alpha + displace(gen), 0, 255);

        color = (alpha << 24) | (red << 16) | (green << 8) | blue;
    } else {
        Triangle t = Triangle(gen, HEIGHT, WIDTH, 1);
        *this = t;
    }
}

bool Triangle::isIn(std::array<int, 2> point) {
    Triangle t1 = Triangle(v1, v2, point);
    Triangle t2 = Triangle(v1, point, v3);
    Triangle t3 = Triangle(point, v2, v3);
    return (getArea() == t1.getArea() + t2.getArea() + t3.getArea());
}

int Triangle::getArea() {
    return abs(v1[0] * (v2[1] - v3[1]) + v2[0] * (v3[1] - v1[1]) + v3[0] * (v1[1] - v2[1]));
}