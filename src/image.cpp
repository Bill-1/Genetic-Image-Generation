#include "image.h"
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

void Image::drawTriangle(const Image &target, Triangle &t, double &loss) {
    std::vector<std::array<int, 2>> nodes;
    std::stack<std::array<int, 2>> st;
    nodes.push_back(t.v1);
    nodes.push_back(t.v2);
    nodes.push_back(t.v3);
    st.push(t.v1);
    st.push(t.v2);
    st.push(t.v3);
    visited[t.v1[0]][t.v1[1]] = true;
    visited[t.v2[0]][t.v2[1]] = true;
    visited[t.v3[0]][t.v3[1]] = true;

    int xx[] = {-1, 0, 1};
    int yy[] = {-1, 0, 1};
    while (!st.empty()) {
        std::array<int, 2> top = st.top();
        st.pop();
        int &color = pixels[top[0]][top[1]];
        int red = (color >> 16) & 0xFF;
        int green = (color >> 8) & 0xFF;
        int blue = color & 0xFF;

        int t_color = t.color;
        int currRed = (t_color >> 16) & 0xFF;
        int currGreen = (t_color >> 8) & 0xFF;
        int currBlue = t_color & 0xFF;

        const int &targetColor = target.getPixel(top[0], top[1]);
        int redTarget = (targetColor >> 16) & 0xFF;
        int greenTarget = (targetColor >> 8) & 0xFF;
        int blueTarget = targetColor & 0xFF;

        loss -= ((redTarget - red) * (redTarget - red));
        loss -= ((greenTarget - green) * (greenTarget - green));
        loss -= ((blueTarget - blue) * (blueTarget - blue));

        red = (currRed * 100 + red * 155) / 255;
        green = (currGreen * 100 + green * 155) / 255;
        blue = (currBlue * 100 + blue * 155) / 255;

        loss += ((redTarget - red) * (redTarget - red));
        loss += ((greenTarget - green) * (greenTarget - green));
        loss += ((blueTarget - blue) * (blueTarget - blue));

        color = (red << 16) | (green << 8) | blue;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                int X = xx[i] + top[0];
                int Y = yy[j] + top[1];
                if (X >= 0 && X < HEIGHT && Y >= 0 && Y < WIDTH && visited[X][Y] == false && t.isIn({X, Y})) {
                    visited[X][Y] = true;
                    std::array<int, 2> next = {X, Y};
                    st.push(next);
                    nodes.push_back(next);
                }
            }
        }
    }

    for (std::array<int, 2> node : nodes) {
        visited[node[0]][node[1]] = false;
    }

}

double Image::tryDraw(const Image &target, Triangle &t, double &loss) {
    double curr = loss;
    long long c1_r = 0, c2_r = 0, c1_g = 0, c2_g = 0, c1_b = 0, c2_b = 0;
    std::vector<std::array<int, 2>> nodes;
    std::stack<std::array<int, 2>> st;
    nodes.push_back(t.v1);
    nodes.push_back(t.v2);
    nodes.push_back(t.v3);
    st.push(t.v1);
    st.push(t.v2);
    st.push(t.v3);
    visited[t.v1[0]][t.v1[1]] = true;
    visited[t.v2[0]][t.v2[1]] = true;
    visited[t.v3[0]][t.v3[1]] = true;

    int xx[] = {-1, 0, 1};
    int yy[] = {-1, 0, 1};
    while (!st.empty()) {
        std::array<int, 2> top = st.top();
        st.pop();
        int &color = pixels[top[0]][top[1]];
        int red = (color >> 16) & 0xFF;
        int green = (color >> 8) & 0xFF;
        int blue = color & 0xFF;
        const int &targetColor = target.getPixel(top[0], top[1]);
        int redTarget = (targetColor >> 16) & 0xFF;
        int greenTarget = (targetColor >> 8) & 0xFF;
        int blueTarget = targetColor & 0xFF;
        c1_r += red;
        c2_r += (red - redTarget);
        c1_g += green;
        c2_g += (green - greenTarget);
        c1_b += blue;
        c2_b += (blue - blueTarget);
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                int X = xx[i] + top[0];
                int Y = yy[j] + top[1];
                if (X >= 0 && X < HEIGHT && Y >= 0 && Y < WIDTH && visited[X][Y] == false && t.isIn({X, Y})) {
                    visited[X][Y] = true;
                    std::array<int, 2> next = {X, Y};
                    st.push(next);
                    nodes.push_back(next);
                }
            }
        }
    }
    long long n = nodes.size();
    int idealRed = (int) ((c1_r * 100 - c2_r * 255) / (n * 100));
    int idealGreen = (int) ((c1_g * 100 - c2_g * 255) / (n * 100));
    int idealBlue = (int) ((c1_b * 100 - c2_b * 255) / (n * 100));
    idealRed = std::max(0, std::min(255, idealRed));
    idealGreen = std::max(0, std::min(255, idealGreen));
    idealBlue = std::max(0, std::min(255, idealBlue));
    int currRed = (t.color >> 16) & 0xFF;
    int currGreen = (t.color >> 8) & (0xFF);
    int currBlue = (t.color) & 0xFF;
    currRed = (idealRed - currRed) * 0.05 + currRed;
    currGreen = (idealGreen - currGreen) * 0.05 + currGreen;
    currBlue = (idealBlue - currBlue) * 0.05 + currBlue;
    currRed = std::max(0, std::min(255, currRed));
    currGreen = std::max(0, std::min(255, currGreen));
    currBlue = std::max(0, std::min(255, currBlue));
    t.color = ((currRed << 16) | (currGreen << 8) | currBlue);
    for (std::array<int, 2> node : nodes) {
        visited[node[0]][node[1]] = false;
        int &color = pixels[node[0]][node[1]];
        int red = (color >> 16) & 0xFF;
        int green = (color >> 8) & 0xFF;
        int blue = color & 0xFF;
        const int &targetColor = target.getPixel(node[0], node[1]);
        int redTarget = (targetColor >> 16) & 0xFF;
        int greenTarget = (targetColor >> 8) & 0xFF;
        int blueTarget = targetColor & 0xFF;
        curr -= ((redTarget - red) * (redTarget - red));
        curr -= ((greenTarget - green) * (greenTarget - green));
        curr -= ((blueTarget - blue) * (blueTarget - blue));
        red = (currRed * 100 + red * 155) / 255;
        green = (currGreen * 100 + green * 155) / 255;
        blue = (currBlue * 100 + blue * 155) / 255;
        curr += ((redTarget - red) * (redTarget - red));
        curr += ((greenTarget - green) * (greenTarget - green));
        curr += ((blueTarget - blue) * (blueTarget - blue));
    }
    return curr;
}