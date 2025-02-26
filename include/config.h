#ifndef CONFIG_H
#define CONFIG_H

#include <random>
#include <vector>

extern std::vector<std::vector<bool>> visited;

extern int HEIGHT;
extern int WIDTH;
extern double initial_loss;

constexpr int GENERATIONS = 1000;
constexpr int TOURNAMENT_SIZE = 5;
constexpr int POPULATION_SIZE = 100;
constexpr int INITIAL_CHROMOSOME = 50;
constexpr double STRUCTURE_RATE = 0.05;
constexpr double MUTATION_RATE = 0.1;

extern std::random_device rd;
extern std::mt19937 gen;
extern std::uniform_real_distribution<> dis;

#endif 
