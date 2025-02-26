#include <filesystem>
#include <iostream>
#include <image.h>
#include "genetic_algorithm.h"

int main() {
    std::string name = "night";
    std::string readingPath = "../assets/" + name + ".png"; 
    Image target = Image::load(readingPath);
    HEIGHT = target.getHeight();
    WIDTH = target.getWidth();
    visited.resize(HEIGHT, std::vector<bool>(WIDTH, false));

    GeneticAlgorithm ga = GeneticAlgorithm(target);
    ga.getGeneticImage(name);
}