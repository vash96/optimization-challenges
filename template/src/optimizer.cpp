#include <bits/stdc++.h>
#include "utils.cpp"    // ScoreManager, Rand
using namespace std;
constexpr array<const char*, 1> ValidNames = {{"ADD-VALID-NAMES!!!"}};
string ProblemName;
uint64_t SEED = 0;
size_t CANDIDATE_MOVES = 0;
double TIME_LIMIT = oo;

bool MINIMIZE = false;


using ScoreType = int64_t; // CHANGE IF NEEDED




void ArgSanitize(int, char**);
void ReadInput(string);
void ReadSolution(string);
void DoMagic();
ScoreType GetScore(/*const SolutionType&*/);
ScoreType DeltaCost(/*const SolutionType&, const MoveType&*/);
void ApplyMove(/*SolutionType&, const MoveType&*/);
void PrintSolution(string);





// SolutionType solution


int main(int argc, char** argv)
{
    ios_base::sync_with_stdio(false);
    ArgSanitize(argc, argv);

    ReadInput( string("input/") + ProblemName + ".txt" );
    ReadSolution( string("output/") + ProblemName + ".greedy" );
    DoMagic();
    PrintSolution( string("output/") + ProblemName + ".opt" );

    return 0;
}


void ArgSanitize(int argc, char** argv)
{
    for(int i=1; i<argc; ) {
        string arg = argv[i];
        if(arg.substr(0, 2) == "--") {
            arg = arg.substr(2);
            if(arg == "problem") {
                if(i+1 < argc) {
                    ProblemName = argv[i+1];
                    i += 2;

                    if(find(ValidNames.begin(), ValidNames.end(), ProblemName)
                        == ValidNames.end()
                    ) {
                        cerr << "Problem name is not valid!" << endl;
                        exit(1);
                    }
                }else {
                    cerr << "Missing name after `--problem` argument!" << endl;
                    exit(1);
                }
            }else if(arg == "seed") {
                if(i+1 < argc) {
                    SEED = atoi(argv[i+1]);
                    i += 2;
                }else {
                    cerr << "Missing seed after `--seed` argument!" << endl;
                    exit(1);
                }
            }else if(arg == "time-limit") {
                if(i+1 < argc) {
                    TIME_LIMIT = atoi(argv[i+1]);
                    i += 2;
                }else {
                    cerr << "Missing time-limit after `--time-limit` argument!" << endl;
                    exit(1);
                }
            }else if(arg == "cand-limit") {
                if(i+1 < argc) {
                    CANDIDATE_MOVES = atoi(argv[i+1]);
                    i += 2;
                }else {
                    cerr << "Missing number of candidates after `--cand-limit` argument!" << endl;
                    exit(1);
                }
            }else if(arg == "minimize") {
                MINIMIZE = true;
                i += 1;
            }else {
                cerr << "Unknown argument --" << arg << ". Valid arguments are: " << endl;
                cerr << "\t--problem <problem name>" << endl;
                cerr << "\t--seed <seed value>" << endl;
                cerr << "\t--time-limit <seconds>" << endl;
                cerr << "\t--cand-limit <number of moves>" << endl;
                cerr << "\t--minimize" << endl;
                exit(1);
            }
        }else {
            cerr << "Wrong form of argument. Must be pre-pended with `--`." << endl;
            exit(1);
        }
    }

    if(CANDIDATE_MOVES == 0) {
        cerr << "Warning! CANDIDATE_MOVES set to 0!" << endl;
    }

    if(SEED == 0) {
        cerr << "Warning! SEED set to 0!" << endl;
    }

    if(TIME_LIMIT == oo) {
        cerr << "Warning! TIME_LIMIT set to infinity!" << endl;
    }

    if(ProblemName == "UNKNOWN") {
        cerr << "Missing problem name. Use `--problem <problem name>` argument!" << endl;
        exit(1);
    }
}

void ReadInput(string filename)
{
    ifstream in(filename);
    if(not in) {
        cerr << "File " << filename << " does not exist!" << endl;
        exit(1);
    }

    cerr << "Reading input from " << filename << " ... " << endl;
}

void ReadSolution(string filename)
{
    ifstream in(filename);
    if(not in) {
        cerr << "File " << filename << " does not exist!" << endl;
        exit(1);
    }

    // Fill "solution" variable
    cerr << "Reading solution from " << filename << " ... " << endl;
}

void DoMagic()
{
    cerr << "Invoking the power of the underworld..." << endl;

    auto t0 = chrono::high_resolution_clock::now();
    auto t1 = t0;

    // SolutionType current = solution;
    ScoreType initialScore = GetScore(/*current*/);
    ScoreType bestScore = initialScore;
    ScoreManager<ScoreType> scoreManager(initialScore, MINIMIZE);

    do {
        // Draw some number of random moves and apply the best one

        vector<ScoreType> delta(CANDIDATE_MOVES, MINIMIZE ? oo : -oo);
        // vector<MoveType> candidate(RAND_LIMIT);
        #pragma omp parallel for
        for(size_t r=0; r<CANDIDATE_MOVES; ++r) {
            // MoveType mv = drawRandomMove();
            delta[r] = DeltaCost(/*current, mv*/);
            // candidate[r] = mv;
        }

        ScoreType bestDelta = MINIMIZE ? oo : -oo;
        // MoveType bestMove = -1;
        for(int r=0; r<CANDIDATE_MOVES; ++r) {
            if(delta[r] * (MINIMIZE ? -1 : +1) > bestDelta * (MINIMIZE ? -1 : +1)) {
                bestDelta = delta[r];
                // bestMove = candidate[r];
            }
        }

        ApplyMove(/*current, bestMove*/);
        scoreManager += bestDelta;

        if(bestDelta > 0) {
            bestScore = scoreManager.score;
	    // solution = current;
        }


        t1 = chrono::high_resolution_clock::now();

    }while( chrono::duration<double>(t1 - t0).count() < TIME_LIMIT );

    cerr << "Optimized score: " << bestScore << "\t\t[Improvement: " << 1. * abs(bestScore - initialScore) / bestScore << "]" << endl;
}


ScoreType GetScore(/*const SolutionType & sol*/)
{
    ScoreType score=0;

    return score;
}

ScoreType DeltaCost(/*const SolutionType & sol, const MoveType & mv*/)
{
    int delta=0;

    return delta;
}

void ApplyMove(/*SolutionType & sol, const MoveType & mv*/)
{

}


void PrintSolution(string filename)
{
    ofstream out(filename);
    
    cerr << "Writing solution to file " << filename << " ... " << endl;

    // Print "solution" variable
}


