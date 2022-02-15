#pragma once
#include <bits/stdc++.h>
using namespace std;
using namespace chrono;

constexpr int64_t oo = numeric_limits<int64_t>::max();

struct PRNG {
    uint64_t seed;

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
    bool MAXIMIZE;
    double threshold;

    ScoreManager(ScoreType score, bool MAXIMIZE=true, double threshold=0.05)
        : score(score), lastUpdate(score), MAXIMIZE(MAXIMIZE), threshold(threshold) { }

    void operator+=(ScoreType delta) {
        score += delta;
        if(static_cast<double>(score-lastUpdate) * (MAXIMIZE ? +1 : -1) >= threshold * lastUpdate * (MAXIMIZE ? +1 : -1)) {
            // If score incremented more than threshold % print
            cerr << "Current score: " << score << "\t\t[Improvement: " << (score-lastUpdate) << "]";
            
            lastUpdate = score;
            threshold *= 0.99;
            
            cerr << "\t\t [Next checkpoint at " << lastUpdate*(1+threshold) << "]\n";
        }
    }
};


template<bool Verbose=true>
struct TimeManager {
    using Time = milliseconds;
    using TimePoint = time_point<high_resolution_clock>;

    Time lifetime, threshold, passed;
    TimePoint epoch;

    TimeManager(Time lifetime, Time threshold=15s) 
        : lifetime(lifetime), threshold(threshold), passed(0s), epoch(high_resolution_clock::now()) {}

    void update() {
        auto now = high_resolution_clock::now();
        passed = duration_cast<Time>(now - epoch);
        
        if( event() ) {
            epoch = now;
            lifetime -= threshold;
            lifetime = max(lifetime, 0ms);
            if constexpr (Verbose) {
                cerr << "You have left to live only " << duration_cast<seconds>(lifetime).count() << " seconds, I'm terribly sorry (or maybe not).\n";
            }
        }
    }

    bool alive() {
        return lifetime > 0s;
    }

    bool event() {
        return passed > threshold;
    }
};