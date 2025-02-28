#include "image.h"
#include <iostream>

Triangle::Triangle() : v1{{0, 0}}, v2{{0, 0}}, v3{{0, 0}}, color(0), active(false) {} 

Triangle::Triangle(std::mt19937 &gen, int H, int W, double rate) {
    active = true;
    int h = H * rate;
    int w = W * rate;
    int displaceX = H - h;
    int displaceY = W - w;
    displaceX = displaceX * dis(gen);
    displaceY = displaceY * dis(gen);
    std::uniform_int_distribution<>disInt(0, h - 1);
    

    std::uniform_int_distribution<>disIn(0, w - 1);
    do {
        v1[0] = disInt(gen);
        v2[0] = disInt(gen);
        v3[0] = disInt(gen);
        v1[1] = disIn(gen);
        v2[1] = disIn(gen);
        v3[1] = disIn(gen);
    } while(getArea() * 8 >= H * W);
    v1[0] += displaceX;
    v2[0] += displaceX;
    v3[0] += displaceX;
    v1[1] += displaceY;
    v2[1] += displaceY;
    v3[1] += displaceY;
    
    std::uniform_int_distribution<>colorDis(0, 255);
    int red = colorDis(gen);
    int green = colorDis(gen);
    int blue = colorDis(gen);
    color = (red << 16) | (green << 8) | blue;
}

Triangle::Triangle(std::array<int, 2> &p1, std::array<int, 2> &p2, std::array<int, 2> &p3) : v1(p1), v2(p2), v3(p3), color(0), active(true) {}

void Triangle::mutate(const Image &target, Image &im, double &loss, std::mt19937 &gen) {
    double chance = dis(gen);
    if (chance >= MUTATION_RATE) 
        return;
    std::uniform_int_distribution<> tweak(-5, 5);
    std::array<int, 2> p1, p2, p3;

    double bestLoss = 1'000'000'000;
    Triangle bestTriangle = Triangle(v1, v2, v3);
    for (int i = 0; i < 2; i++) {
        p1 = v1, p2 = v2, p3 = v3;
        p1[0] = std::min(std::max(0, p1[0] + tweak(gen)), HEIGHT - 1);
        p1[1] = std::min(std::max(0, p1[1] + tweak(gen)), WIDTH - 1);
        p2[0] = std::min(std::max(0, p2[0] + tweak(gen)), HEIGHT - 1);
        p2[1] = std::min(std::max(0, p2[1] + tweak(gen)), WIDTH - 1);
        p3[0] = std::min(std::max(0, p3[0] + tweak(gen)), HEIGHT - 1);
        p3[1] = std::min(std::max(0, p3[1] + tweak(gen)), WIDTH - 1);
        Triangle t = Triangle(p1, p2, p3);
        double currLoss = im.tryDraw(target, t, loss, gen);
        if (bestLoss > currLoss) {
            bestTriangle = t;
            bestLoss = currLoss;
        }
    }

    *this = bestTriangle;
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