#ifndef CONFIG_H
#define CONFIG_H

#include <random>
#include <vector>

extern int HEIGHT;
extern int WIDTH;
extern double initial_loss;

constexpr int GENERATIONS = 1000;
constexpr int TOURNAMENT_SIZE = 3;
constexpr int POPULATION_SIZE = 100;
constexpr int INITIAL_CHROMOSOME = 100;
constexpr double STRUCTURE_RATE = 0.05;

extern double MUTATION_RATE;

extern std::random_device rd;
extern std::mt19937 gen;
extern std::uniform_real_distribution<> dis;

#endif 
