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

void Individual::mutate(const Image &target, std::mt19937 &gen) {

    double chance = dis(gen);
    if (chance < STRUCTURE_RATE) {
        dna.push_back(Triangle(gen, HEIGHT, WIDTH, loss / (initial_loss / (HEIGHT * WIDTH * 3))));
    } else if (chance > (1 - STRUCTURE_RATE)) {
        std::uniform_int_distribution<> rem(0, dna.size() - 1);
        int id = rem(gen);
        while (!dna[id].active) id = rem(gen);
        dna[id].active = false;
    } 
    Image im = Image(HEIGHT, WIDTH);
    this -> loss = initial_loss;
    for (int i = 0; i < dna.size(); i++) {
        if (!dna[i].active) continue;
        dna[i].mutate(target, im, loss, gen);
        im.drawTriangle(target, dna[i], loss);
    }
    this -> loss = this -> loss / (3 * HEIGHT * WIDTH);
}

Image Individual::render(const Image &target) {
    Image im = Image(HEIGHT, WIDTH);
    double L = initial_loss;
    for (int i = 0; i < dna.size(); i++) {
        if (!dna[i].active) continue;
        im.drawTriangle(target, dna[i], L);
    }
    return im;
}