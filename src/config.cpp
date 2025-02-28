#include "config.h"

int HEIGHT = 100;
int WIDTH = 100;    
double initial_loss = 0;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.0, 1.0);