#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>
#include <cstdint>
#include <triangle.h>

class Image {
public:
    Image();
    Image(int height, int wight);

    static Image load(const std::string &filename);

    bool save(const std::string &filename) const;

    int getHeight() const;
    int getWidth() const;

    int getPixel(int x, int y) const;
    void setPixel(int x, int y, int color);

    void drawTriangle(const Image &target, Triangle &t, double &loss);
    double tryDraw(const Image &target, Triangle &t, double &loss, std::mt19937 &gen);
private:
    int height;
    int width;
    std::vector<std::vector<int>> pixels;
    std::vector<std::vector<bool>> visited;
};

#endif