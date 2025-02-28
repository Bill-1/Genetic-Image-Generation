#include "image.h"
#include "point.h"
#include <iostream>
#include <stack>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Image::Image() : width(0), height(0) {}

Image::Image(int h, int w) : height(h), width(w), pixels(h, std::vector<int> (w, (255 << 16) | (255 << 8) | 255)), visited(h, std::vector<bool>(w, false)) {}

Image Image::load(const std::string &filename) {
    int width, height, channels;
    unsigned char* image = stbi_load(filename.c_str(), &width, &height, &channels, 3);
    if (!image) {
        std::cerr << "Error: Could not load image!" << std::endl;
        return {};
    }
    std::cout << "Loading image: " << filename << "\n";
    Image im = Image(height, width);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * 3; 
            int r = image[index];
            int g = image[index + 1];
            int b = image[index + 2];
            initial_loss += (r - 255) * (r - 255);
            initial_loss += (g - 255) * (g - 255);
            initial_loss += (b - 255) * (b - 255);
            int value = (r << 16) | (g << 8) | b;
            im.setPixel(y, x, value);
        }
    }
    return im;
}

bool Image::save(const std::string &filename) const {
    std::cout << "Saving image: " << filename << "\n";
    int channels = 3;
    std::vector<unsigned char> imageData(height * width * channels);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * channels;
            int color = pixels[y][x];

            imageData[index]     = (color >> 16) & 0xFF;
            imageData[index + 1] = (color >> 8)  & 0xFF; 
            imageData[index + 2] = color         & 0xFF; 
        }
    }
    if (!stbi_write_png(filename.c_str(), width, height, channels, imageData.data(), width * channels)) {
        return true;
    } else {
        return false;
    }
}

int Image::getHeight() const { return height; }
int Image::getWidth() const { return width; }

int Image::getPixel(int x, int y) const {
    return pixels[x][y];
}

void Image::setPixel(int x, int y, int value) {
    pixels[x][y] = value;
}

inline int clamp(int value, int minVal, int maxVal) {
    return std::max(minVal, std::min(value, maxVal));
}

void Image::drawTriangle(const Image &target, Triangle &t, double &loss) {
    int x1 = clamp(t.v1[0], 0, HEIGHT - 1);
    int y1 = clamp(t.v1[1], 0, WIDTH - 1);
    int x2 = clamp(t.v2[0], 0, HEIGHT - 1);
    int y2 = clamp(t.v2[1], 0, WIDTH - 1);
    int x3 = clamp(t.v3[0], 0, HEIGHT - 1);
    int y3 = clamp(t.v3[1], 0, WIDTH - 1);

    struct Point { int x; int y; };
    Point pts[3] = { {x1, y1}, {x2, y2}, {x3, y3} };

    std::sort(pts, pts + 3, [](const Point &a, const Point &b) {
        return a.x < b.x;
    });

    auto interpolate = [&](int x, const Point &p0, const Point &p1) -> int {
        if (p1.x == p0.x)
            return p0.y;
        double t = (x - p0.x) / static_cast<double>(p1.x - p0.x);
        return static_cast<int>(p0.y + t * (p1.y - p0.y) + 0.5);
    };

    int triAlpha = (t.color >> 24) & 0xFF;
    double alphaFactor = triAlpha / 255.0;
    int triR = (t.color >> 16) & 0xFF;
    int triG = (t.color >> 8) & 0xFF;
    int triB = t.color & 0xFF;

    for (int x = pts[0].x; x <= pts[2].x; x++) {
        if (x < 0 || x >= HEIGHT)
            continue;

        int ya, yb;
        if (x < pts[1].x) {
            ya = interpolate(x, pts[0], pts[1]);
            yb = interpolate(x, pts[0], pts[2]);
        } else {
            ya = interpolate(x, pts[1], pts[2]);
            yb = interpolate(x, pts[0], pts[2]);
        }
        if (ya > yb)
            std::swap(ya, yb);
        ya = clamp(ya, 0, WIDTH - 1);
        yb = clamp(yb, 0, WIDTH - 1);

        for (int y = ya; y <= yb; y++) {
            int currentColor = pixels[x][y];
            int currA = (currentColor >> 24) & 0xFF; 
            int currR = (currentColor >> 16) & 0xFF;
            int currG = (currentColor >> 8) & 0xFF;
            int currB = currentColor & 0xFF;

            int targetColor = target.getPixel(x, y);
            int targR = (targetColor >> 16) & 0xFF;
            int targG = (targetColor >> 8) & 0xFF;
            int targB = targetColor & 0xFF;

            double oldError = std::pow(targR - currR, 2) + std::pow(targG - currG, 2) + std::pow(targB - currB, 2);
            loss -= oldError;

            int newR = static_cast<int>(alphaFactor * triR + (1 - alphaFactor) * currR);
            int newG = static_cast<int>(alphaFactor * triG + (1 - alphaFactor) * currG);
            int newB = static_cast<int>(alphaFactor * triB + (1 - alphaFactor) * currB);

            int newColor = (255 << 24) | (newR << 16) | (newG << 8) | newB;

            pixels[x][y] = newColor;

            double newError = std::pow(targR - newR, 2) + std::pow(targG - newG, 2) + std::pow(targB - newB, 2);
            loss += newError;
        }
    }
}
