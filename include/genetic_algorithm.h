#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H


#include "individual.h"
#include <string>

class GeneticAlgorithm {
public:
    GeneticAlgorithm(const Image &target);

    void save(const std::string& filename);
    void load(const std::string& filename);

    void run(int &itr, double &prevLoss);

    void getGeneticImage(std::string name);

    Individual doTournamentSelection();

    Individual breed(const Individual &parent1, const Individual &parent2);
private:
    int itr = 0;
    const Image &target;
    std::vector<Individual> population{POPULATION_SIZE};
    std::string folderName;
};
#endif