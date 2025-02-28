# Image Genetic Algorithm

## Overview

This project implements a genetic algorithm designed to evolve images towards a target image. By simulating the process of natural selection, the algorithm iteratively improves a population of images, minimizing the difference between each generated image and the desired target.

## "The Starry Night" after 2500 generations
![alt text](https://github.com/Bill-1/Genetic-Image-Generation/blob/main/assets/night/250.png)
## Features

- **Customizable Parameters**: Adjust population size, chromosome number, mutation rates, and other variables to observe different evolutionary outcomes. These variables can be found in the config.h file.
- **Real-time Monitoring**: Visualize the evolution process with intermediate images displayed at each 10 generations.

## Installation & Usage

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/yourusername/image-genetic-algorithm.git

2. **Navigate to the Project Directory**:
   ```bash
   cd image-genetic-algorithm 
3. **Make a build directory for CMake and navigate to it**:
   ```bash
   mkdir build && cd build
4. **Generate the makefiles**
   ```bash
   cmake ..
5. **Build the program**
   ```bash
   cmake --build . && ./ga_triangles
6. **Monitor the progress**

## How It Works

1. **Initialization**: Generate an initial population of random images.
2. **Mutation**: Apply random alterations to individuals to explore new variations.  
3. **Evaluation**: Assess each image using a fitness function that measures similarity to the target image.
4. **Selection**: Use tournament selection to choose the parents for the next generation.
5. **Crossover**: Breed the parents to produce offspring, introducing genetic diversity.
6. **Iteration**: Repeat.

## Inspiration

This project was inspired from Darwin's theory of evolution, particularly the concept of natural selection. 
I would like to thank Sebastian Proost's work for the mutation mechanism in this project.

## Optimization

This project employs several performance optimizations to address the computational intensity of the image genetic algorithm. Multithreading is used to accelerate the evaluation of candidate solutions, allowing multiple processes to run concurrently and significantly reducing overall runtime. Additionally, the initial approach for rendering triangles relied on flood-filling, which proved to be slow. Switching to a scanline algorithm has provided a more efficient drawing alternative, improving rendering speed considerably. Also, the training process leverages serialization and deserialization to persist and restore the state of the algorithm. This mechanism enables training sessions to be paused and resumed from saved checkpoints.





