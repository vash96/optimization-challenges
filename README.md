# optimization-challenges
Code template for optimization challenges (e.g. Hashcode, Reply Code, etc).

# Introduction
Optimization challanges consist of solving hard optimization problems in a limited amount of time.
Together with the problem there is a score function which measures the goodness of the found solution.
The goal is to maximize/minimize the goodness.

Popular techniques to tackle optimization problems revolve around greedy algorithms and local search meta-heuristics.
This repo contains a template to structure the code for such a challenge.

# The structure
The directory hierarchy is the following:
```bash
.
├── bin
├── input
├── Makefile
├── output
└── src
    ├── greedy.cpp
    ├── optimizer.cpp
    └── utils.cpp
```