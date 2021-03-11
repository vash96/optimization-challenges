#include <bits/stdc++.h>
#include "utils.cpp"    // ScoreManager, Rand
using namespace std;
constexpr array<const char*, 6> validNames = {{"a", "b", "c", "d", "e", "f"}};
string problemName;
int RAND_LIMIT, TIME_LIMIT;


void argSanitize(int, char**);
void readInput(string);
void readSolution(string);
void doMagic();
int getScore(/*const SolutionType&*/);
int deltaCost(/*const SolutionType&, const MoveType&*/);
void applyMove(/*SolutionType&, const MoveType&*/);
void printSolution(string);





// SolutionType solution


int main(int argc, char** argv)
{
    ios_base::sync_with_stdio(false);
    argSanitize(argc, argv);

    readInput( string("input/") + problemName + ".txt" );
    readSolution( string("output/") + problemName + ".greedy" );
    doMagic();
    printSolution( string("output/") + problemName + ".opt" );

    return 0;
}


void argSanitize(int argc, char** argv)
{
    if(argc != 4) {
        cerr << "Wrong number of parameters..." << endl;
        cerr << "Usage: " << argv[0] << " <problem-name> <number-of-rand-draws> <time-limit-in-seconds>" << endl;
        exit(1);
    }

    problemName = argv[1];
    RAND_LIMIT = atoi(argv[2]);
    TIME_LIMIT = atoi(argv[3]);

    if(find(validNames.begin(), validNames.end(), problemName.c_str()) 
        == validNames.end()
    ) {
        cerr << "Problem name is not valid. Check valid name list!" << endl;
        exit(1);
    }
}

void readInput(string filename)
{
    ifstream in(filename);
    if(not in) {
        cerr << "File " << filename << " does not exist!" << endl;
        exit(1);
    }

    cerr << "Reading input from " << filename << " ... " << endl;
}

void readSolution(string filename)
{
    ifstream in(filename);
    if(not in) {
        cerr << "File " << filename << " does not exist!" << endl;
        exit(1);
    }

    // Fill "solution" variable
}

void doMagic()
{
    cerr << "Invoking the power of the underworld..." << endl;

    auto t0 = chrono::high_resolution_clock::now();
    auto t1 = t0;

    // SolutionType current, best = current;
    int initialScore = getScore(/*current*/);
    int bestScore = initialScore;
    ScoreManager scoreManager(initialScore);

    do {
        // Draw some number of random moves and apply the best one

        vector<int> delta(RAND_LIMIT, -1);
        // vector<MoveType> candidate(RAND_LIMIT);
        #pragma omp parallel for
        for(int r=0; r<RAND_LIMIT; ++r) {
            // MoveType mv = drawRandomMove();
            delta[r] = deltaCost(/*current, mv*/);
            // candidate[r] = mv;
        }

        int bestDelta = 0;
        // MoveType bestMove = -1;
        for(int r=0; r<RAND_LIMIT; ++r) {
            if(delta[r] > bestDelta) {
                bestDelta = delta[r];
                // bestMove = candidate[r];
            }
        }

        applyMove(/*current, bestMove*/);
        scoreManager += bestDelta;

        if(bestDelta > 0) {
            bestScore = scoreManager.score;
        }


        t1 = chrono::high_resolution_clock::now();

    }while( chrono::duration<double>(t1 - t0).count() < TIME_LIMIT );

    cerr << "Optimized score: " << bestScore << "\t\t[+" << (bestScore - initialScore) << "]" << endl;
}


int getScore(/*const SolutionType & sol*/)
{
    int score=0;

    return score;
}

int deltaCost(/*const SolutionType & sol, const MoveType & mv*/)
{
    int delta=0;

    return delta;
}

void applyMove(/*SolutionType & sol, const MoveType & mv*/)
{

}


void printSolution(string filename)
{
    ofstream out(filename);
    
    cerr << "Writing solution to file " << filename << " ... " << endl;

    // Print "solution" variable
}


