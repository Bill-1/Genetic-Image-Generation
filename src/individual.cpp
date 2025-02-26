#include "individual.h"
#include "image.h"
#include <iostream>

Individual::Individual() {
    for (int i = 0; i < INITIAL_CHROMOSOME; i++) {
        dna.push_back(Triangle(HEIGHT, WIDTH));
    }
}

Individual::Individual(std::vector<Triangle> &dna) : dna(dna) {
    loss = initial_loss;
}

double Individual::getLoss() const{
    return loss;
}

void Individual::mutate(const Image &target) {
    double chance = dis(gen);
    if (chance < STRUCTURE_RATE) {
        dna.push_back(Triangle(HEIGHT, WIDTH));
    } else if (chance > (1 - STRUCTURE_RATE)) {
        std::uniform_int_distribution<> rem(0, dna.size() - 1);
        int id = rem(gen);
        while (!dna[id].active) id = rem(gen);
        dna[id].active = false;
    }
    
    Image im = Image(HEIGHT, WIDTH);
    this -> loss = initial_loss;
    for (int i = 0; i < dna.size(); i++) {
        std::cout << i << "th chromosome is in the mutating process\n";
        if (!dna[i].active) continue;
        dna[i].mutate(target, im, loss);
        im.drawTriangle(target, dna[i], loss);
    }
    im.save("../assets/output.png");
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