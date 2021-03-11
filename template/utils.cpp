#pragma once
#include <bits/stdc++.h>
using namespace std;

random_device rd;
mt19937 rnd(rd());


// Print the score (+increment) everytime the score gets an increment 
// of at least some threshold from last print 
struct ScoreManager {
    int score;
    int lastUpdate;
    double threshold;

    ScoreManager(int score, double threshold=0.05)
        : score(score), lastUpdate(score), threshold(threshold) { }

    void operator+=(int delta) {
        score += delta;
        if(static_cast<double>(score-lastUpdate) >= threshold * lastUpdate) {
            // If score incremented more than threshold % print
            cerr << "Current score: " << score << "\t\t[Increment +" << (score-lastUpdate) << "]\n";
            lastUpdate = score;
        }
    }
};
