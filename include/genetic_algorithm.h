#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H


#include "individual.h"
#include <string>

class GeneticAlgorithm {
public:
    GeneticAlgorithm(const Image &target);

    void run(int &itr);

    void getGeneticImage(std::string name);

    Individual doTournamentSelection();

    Individual breed(const Individual &parent1, const Individual &parent2);
private:
    const Image &target;
    std::vector<Individual> population{POPULATION_SIZE};
    std::string folderName;
};
#endif