#include <filesystem>
#include <iostream>
#include <image.h>
#include <csignal>
#include "genetic_algorithm.h"

GeneticAlgorithm* ga = nullptr;
std::string name;

void signalHandler(int signum) {
    std::cout << "\n[INFO] Interrupt signal (" << signum << ") received. Saving state...\n";
    if (ga) {
        ga->save("../assets/" + name + "/genetic_algorithm_state.dat");
    }
    exit(signum);
}

int main() {
    
    std::signal(SIGINT, signalHandler);

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    name = "night";
    std::string readingPath = "../assets/" + name + ".png"; 
    Image target = Image::load(readingPath);
    HEIGHT = target.getHeight();
    WIDTH = target.getWidth();

    GeneticAlgorithm gaInstance(target);
    ga = &gaInstance;

    gaInstance.load("../assets/" + name + "/genetic_algorithm_state.dat");
    gaInstance.getGeneticImage(name);

}