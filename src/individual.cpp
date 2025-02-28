#include "individual.h"
#include "image.h"
#include <iostream>

Individual::Individual() {
    loss = initial_loss / (HEIGHT * WIDTH * 3); 
    for (int i = 0; i < INITIAL_CHROMOSOME; i++) {
        dna.push_back(Triangle(gen, HEIGHT, WIDTH, 1));
    }
}

Individual::Individual(std::vector<Triangle> &dna) : dna(dna) {
    loss = initial_loss / (HEIGHT * WIDTH * 3);
}

double Individual::getLoss() const{
    return this -> loss;
}

void Individual::mutate(std::mt19937 &gen) {
    std::vector<Triangle> mutatedTriangles;
    for (int i = 1; i <= POPULATION_SIZE * STRUCTURE_RATE; i++) {
        std::uniform_int_distribution<> rem(0, dna.size() - 1);
        int id = rem(gen);
        dna[id].mutate(gen);
        mutatedTriangles.push_back(dna[id]);
    }
    std::swap(mutatedTriangles[0], mutatedTriangles[1]);
}

Image Individual::render(const Image &target) {
    Image im = Image(HEIGHT, WIDTH);
    loss = initial_loss;
    for (int i = 0; i < dna.size(); i++) {
        im.drawTriangle(target, dna[i], loss);
    }
    loss /= (HEIGHT * WIDTH * 3);
    return im;
}