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

`Makefile` can compile also a debugging version of the code.

# Usage
`greedy.cpp` contains a template for a greedy/randomized simple algorithm. It reads an input file `<problem>` from `input/<problem>` 
and writes a solution to `output/<problem>.greedy`  

The usage is:
```bash
make greedy
./bin/greedy --problem <problem> [--seed <seed-value>]
```
Options in brackets are optional.


`optimizer.cpp` contains a template for local search (hill climbing) algorithm. It reads an input file `<filename>` from `input/<filename>`,
a corresponding greedy solution from `output/<filename>.greedy` and writes an optimized solution to `output/<filename>.opt`  

The usage is:
```bash
make opt
./bin/opt --problem <problem> [--seed <seed-value>, --time-limit <seconds>, --cand-limit <number-of-moves>, --minimize]
```

Options in brackets are optional.


# Options
`--problem <problem>`: name of the problem to be solved. There must to be a file `input/<problem>`.  
`--seed <seed-value>`: seed to be set at initialization. Optional.  
`--time-limit`: number of seconds to limit the search. Optional. Default = 10^18 seconds (i.e. infinity).  
`--cand-limit`: candidate moves for hill-climbing are chosen randomly. The argument limits the number of moves to try. Optional. Default = 1.  
`--minimize`: usually, optimization challenges are of the form of maximization of some score. This options enables the minimization of the score.  