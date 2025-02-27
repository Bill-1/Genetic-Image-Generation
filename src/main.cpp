#include <filesystem>
#include <iostream>
#include <image.h>
#include "genetic_algorithm.h"

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    std::string name = "night";
    std::string readingPath = "../assets/" + name + ".png"; 
    Image target = Image::load(readingPath);
    HEIGHT = target.getHeight();
    WIDTH = target.getWidth();

    GeneticAlgorithm ga = GeneticAlgorithm(target);
    ga.getGeneticImage(name);
}