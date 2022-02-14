#pragma once
#include <bits/stdc++.h>
using namespace std;

constexpr int64_t oo = numeric_limits<int64_t>::max();

struct PRNG {
    uint64_t seed{};

    PRNG(uint64_t seed=0) : seed(seed) {}


    uint64_t operator()()
    {
        uint64_t z = (seed += UINT64_C(0x9E3779B97F4A7C15));
        z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
        z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
        return z ^ (z >> 31);
    }
};

// Print the score (+increment) everytime the score gets an increment 
// of at least some threshold from last print 
template<typename ScoreType=int64_t>
struct ScoreManager {
    ScoreType score;
    ScoreType lastUpdate;
    double threshold;

    ScoreManager(ScoreType score, double threshold=0.05)
        : score(score), lastUpdate(score), threshold(threshold) { }

    void operator+=(ScoreType delta) {
        score += delta;
        if(static_cast<double>(score-lastUpdate) >= threshold * lastUpdate) {
            // If score incremented more than threshold % print
            cerr << "Current score: " << score << "\t\t[Increment +" << (score-lastUpdate) << "]\n";
            lastUpdate = score;
        }
    }
};