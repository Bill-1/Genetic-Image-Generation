#include "image.h"
#include "genetic_algorithm.h"
#include <filesystem>
#include <iostream>
#include <thread_pool.h>
#include <fstream>

GeneticAlgorithm::GeneticAlgorithm(const Image &target) : target(target) {
    population.resize(POPULATION_SIZE);
    for (int i = 0; i < POPULATION_SIZE; i++) {
        population[i] = Individual();
    }
    std::cout << "Population created\n";
}

void GeneticAlgorithm::save(const std::string& filename) {
    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        std::cerr << "[ERROR] Cannot open file for writing: " << filename << std::endl;
        return;
    }
    out.write(reinterpret_cast<const char*>(&itr), sizeof(itr));

    size_t size = population.size();
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));

    if (size > 0) {
        for (const Individual &ind : population) {
            size_t numTriangles = ind.dna.size();
            out.write(reinterpret_cast<const char*>(&numTriangles), sizeof(numTriangles));
            for (const Triangle &tri : ind.dna) {
                // Write each triangle's components.
                out.write(reinterpret_cast<const char*>(tri.v1.data()), 2 * sizeof(int));
                out.write(reinterpret_cast<const char*>(tri.v2.data()), 2 * sizeof(int));
                out.write(reinterpret_cast<const char*>(tri.v3.data()), 2 * sizeof(int));
                out.write(reinterpret_cast<const char*>(&tri.color), sizeof(tri.color));
            }
        }
    }
    out.close();
    std::cout << "[INFO] Saved state to " << filename << std::endl;
}

void GeneticAlgorithm::load(const std::string& filename) {
    if (!std::filesystem::exists(filename)) {
        std::cout << "[INFO] " << filename << " does not exist. Skipping load." << std::endl;
        return;
    }

    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        std::cerr << "[ERROR] Cannot open file for reading: " << filename << std::endl;
        return;
    }

    in.read(reinterpret_cast<char*>(&itr), sizeof(itr));
    if (!in) {
        std::cerr << "[ERROR] Failed to read iteration variable." << std::endl;
        return;
    }

    size_t popSize = 0;
    in.read(reinterpret_cast<char*>(&popSize), sizeof(popSize));
    if (!in) {
        std::cerr << "[ERROR] Failed to read population size." << std::endl;
        return;
    }

    std::cout << "[DEBUG] Loading population of size: " << popSize << std::endl;
    population.clear();
    population.resize(popSize);

    for (size_t i = 0; i < popSize; i++) {
        size_t numTriangles = 0;
        in.read(reinterpret_cast<char*>(&numTriangles), sizeof(numTriangles));
        if (!in) {
            std::cerr << "[ERROR] Failed to read triangle count for individual " << i << std::endl;
            return;
        }

        Individual ind = Individual();
        ind.dna.clear();
        ind.dna.resize(numTriangles);
        for (size_t j = 0; j < numTriangles; j++) {
            std::array<int, 2> p1 = {0, 0};
            std::array<int, 2> p2 = {0, 0};
            std::array<int, 2> p3 = {0, 0};
            Triangle tri(p1, p2, p3); 
            in.read(reinterpret_cast<char*>(tri.v1.data()), 2 * sizeof(int));
            in.read(reinterpret_cast<char*>(tri.v2.data()), 2 * sizeof(int));
            in.read(reinterpret_cast<char*>(tri.v3.data()), 2 * sizeof(int));
            in.read(reinterpret_cast<char*>(&tri.color), sizeof(tri.color));

            if (!in) {
                std::cerr << "[ERROR] Failed to read triangle data for individual " << i << std::endl;
                return;
            }
            ind.dna[j] = tri;
        }

        population[i] = (ind);
    }
    
    in.close();
    std::cout << "[INFO] Loaded state from " << filename << " with iteration: " << itr << std::endl;
}

void GeneticAlgorithm::run(int &itr, double &prevLoss) {
    size_t numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 2;
    ThreadPool pool(numThreads);
    std::vector<std::pair<double, Individual*>> lossPopulation ;
    for (int i = 0; i < population.size(); i++) {
        pool.enqueue([this, i](){
            thread_local std::mt19937 local_gen(std::random_device{}());
            population[i].mutate(local_gen);
            population[i].render(target);
        });
    }
    
    pool.wait();
    
    for (int i = 0; i < POPULATION_SIZE; i++) {
        double loss = population[i].getLoss();
        lossPopulation.push_back({loss, &population[i]});
    }
    std::sort(lossPopulation.begin(), lossPopulation.end());
    Individual *currBest = lossPopulation[0].second;

    
    std::cout << "Generation " << std::to_string(itr) << " is over with loss " << currBest->getLoss() << " with " << currBest->dna.size() << " triangles\n"; 

    if (itr % 10 == 0) {
        std::string filePath = "../assets/" + folderName + "/" + std::to_string(itr / 10) + ".png";
        Image best = currBest -> render(target);
        if (best.save(filePath)) {
            std::cout << "Success\n";
        } else {
            std::cerr << "Failed to save output image.\n";
        }
    }
    std::vector<Individual> children{POPULATION_SIZE};
    for (int i = 0; i < POPULATION_SIZE; i++) {
        Individual father = doTournamentSelection();
        Individual mother = doTournamentSelection();
        children[i] = breed(father, mother);
    }
    population = children;
}

void GeneticAlgorithm::getGeneticImage(std::string name) {
    double prevLoss = initial_loss;
    folderName = name;
    std::filesystem::path dir = "../assets/" + name;
    if (std::filesystem::create_directory(dir)) {
        std::cout << "Directory created successfully.\n";
    } else {
        std::cout << "Failed to create directory or already exists.\n";
    }
    while (true) {
        itr++;
        run(itr, prevLoss);
    }
}

Individual GeneticAlgorithm::doTournamentSelection() {
    std::uniform_int_distribution<> pick(0, POPULATION_SIZE - 1);
    const Individual *best = nullptr;
    double bestLoss = 1'000'000'000;
    for (int i = 1; i <= TOURNAMENT_SIZE; i++) {
        int id = pick(gen);
        const Individual *candidate = &population[id];
        double candidateLoss = candidate -> getLoss();
        if (candidateLoss < bestLoss) {
            bestLoss = candidateLoss;
            best = candidate;
        }
    }
    return *best;
}

Individual GeneticAlgorithm::breed(const Individual &parent1, const Individual &parent2) {
    std::vector<Triangle> childDNA;
    size_t maxSize = std::max(parent1.dna.size(), parent2.dna.size());
    for (size_t i = 0; i < maxSize; ++i) {
        if (dis(gen) < 0.5) {
            if (i < parent1.dna.size()) {
                childDNA.push_back(parent1.dna[i]);
            } else {
                childDNA.push_back(parent2.dna[i]);
            }
        } else {
            if (i < parent2.dna.size()) {
                childDNA.push_back(parent2.dna[i]);
            } else {
                childDNA.push_back(parent1.dna[i]);
            }
        }
    }
    return Individual(childDNA);
     
}