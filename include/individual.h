#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <triangle.h>
#include <vector>

class Individual {
public:
    std::vector<Triangle> dna;

    Individual();
    Individual(std::vector<Triangle> &dna);

    double getLoss() const;
    void mutate(std::mt19937 &gen);

    Image render(const Image &target);
private:
    double loss;
};
#endif