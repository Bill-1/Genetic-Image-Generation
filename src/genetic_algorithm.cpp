#include "image.h"
#include "genetic_algorithm.h"
#include <filesystem>
#include <iostream>

GeneticAlgorithm::GeneticAlgorithm(const Image &target) : target(target) {
    for (int i = 0; i < POPULATION_SIZE; i++) {
        population[i] = Individual();
    }
    std::cout << "Population created\n";
}

void GeneticAlgorithm::run(int &itr) {
    Individual *currBest = &population[0]; 
    double currBestLoss = population[0].getLoss();
   
    for (int i = 0; i < POPULATION_SIZE; i++) {
        population[i].mutate(target);
        std::cout << i << "th mutated\n";
        if (population[i].getLoss() < currBestLoss) {
            currBestLoss = population[i].getLoss();
            currBest = &population[i]; 
        }
    }
    std::cout << "Is this right?\n";
    std::vector<Individual> children{POPULATION_SIZE};
    for (int i = 0; i < POPULATION_SIZE; i++) {
        Individual father = doTournamentSelection();
        Individual mother = doTournamentSelection();
        children[i] = breed(father, mother);
    }
    population = children;

    std::cout << "Generation " << std::to_string(itr) << " is over with loss " << currBest->getLoss() << "\n"; 
    if (itr % 10 == 0) {
        std::string filePath = "../assets/" + folderName + "/" + std::to_string(itr / 10) + ".png";
        Image best = currBest -> render(target);
        if (best.save(filePath)) {
            std::cout << "Success\n";
        } else {
            std::cerr << "Failed to save output image.\n";
        }
    }
}

void GeneticAlgorithm::getGeneticImage(std::string name) {
    folderName = name;
    std::filesystem::path dir = "../assets/" + name;
    if (std::filesystem::create_directory(dir)) {
        std::cout << "Directory created successfully.\n";
    } else {
        std::cout << "Failed to create directory or already exists.\n";
    }
    int itr = 0;
    while (true) {
        itr++;
        run(itr);
    }
}

Individual GeneticAlgorithm::doTournamentSelection() {
    std::uniform_int_distribution<> pick(0, POPULATION_SIZE - 1);
    const Individual *best = nullptr;
    double bestLoss = 1'000'000;
    for (int i = 1; i <= TOURNAMENT_SIZE; i++) {
        int id = pick(gen);
        const Individual *candidate = &population[id];
        double candidateLoss = candidate -> getLoss();
        if (candidateLoss < bestLoss) {
            candidateLoss = bestLoss;
            best = candidate;
        }
    }
    return *best;
}

Individual GeneticAlgorithm::breed(const Individual &parent1, const Individual &parent2) {
    const Individual *smallParent;
    const Individual *largeParent;
    if (parent1.dna.size() <= parent2.dna.size()) {
        smallParent = &parent1;
        largeParent = &parent2;
    } else {
        smallParent = &parent2;
        largeParent = &parent1;
    }
    
    if (smallParent->dna.empty()) {
        return Individual(); 
    }
    
    size_t minSize = smallParent->dna.size();
    std::uniform_int_distribution<> index(0, minSize - 1);
    size_t pivot = index(gen);
   
    std::vector<Triangle> childDNA;
    
    for (size_t i = 0; i < pivot; ++i) {
        childDNA.push_back(smallParent->dna[i]);
    }
    
    for (size_t i = pivot; i < largeParent->dna.size(); ++i) {
        childDNA.push_back(largeParent->dna[i]);
    }
    
    return Individual(childDNA);
}