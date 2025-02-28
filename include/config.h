#ifndef CONFIG_H
#define CONFIG_H

#include <random>
#include <vector>

extern int HEIGHT;
extern int WIDTH;
extern double initial_loss;

constexpr int TOURNAMENT_SIZE = 5;
constexpr int POPULATION_SIZE = 150;
constexpr int INITIAL_CHROMOSOME = 150;
constexpr double STRUCTURE_RATE = 0.05;
constexpr double MUTATION_RATES[] = {0.32, 0.64, 0.96};

extern std::random_device rd;
extern std::mt19937 gen;
extern std::uniform_real_distribution<> dis;

#endif 
