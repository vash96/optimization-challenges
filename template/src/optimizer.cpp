#include <bits/stdc++.h>
#include "utils.cpp"    // ScoreManager, TimeManager, Random
using namespace std;
using namespace chrono;

constexpr array<const char*, 1> ValidNames = {{"ADD-VALID-NAMES!!!"}};
string ProblemName;
uint64_t SEED = 0;
size_t CANDIDATE_MOVES = 1;
seconds TIME_LIMIT(oo);

bool MAXIMIZE = true;


using ScoreType = int64_t; // CHANGE IF NEEDED
using SolutionType = vector<int>; // CHANGE IF NEEDED
using MoveType = pair<int, int>; // CHANGE IF NEEDED




void ArgSanitize(int, char**);
void ReadInput(string);
void ReadSolution(string);
void DoMagic();
ScoreType GetScore(const SolutionType&);
MoveType DrawRandomMove(PRNG&);
ScoreType DeltaCost(const SolutionType&, const MoveType&);
bool IsBetterMove(ScoreType, ScoreType); // CHANGE THIS TO ADAPT TO SIMULATED ANNEALING
void ApplyMove(SolutionType&, const MoveType&);
void PrintSolution(string);





SolutionType best, initial;


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
                    TIME_LIMIT = seconds(atoi(argv[i+1]));
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
                MAXIMIZE = false;
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

    if(CANDIDATE_MOVES == 1) {
        cerr << "Warning! CANDIDATE_MOVES set to 1!" << endl;
    }

    if(SEED == 0) {
        cerr << "Warning! SEED set to 0!" << endl;
    }

    if(TIME_LIMIT == seconds(oo)) {
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

    // Fill "initial" variable
    cerr << "Reading solution from " << filename << " ... " << endl;
}

void DoMagic()
{
    cerr << "Invoking the power of the underworld..." << endl;

    SolutionType current = best = initial;
    ScoreType initialScore = GetScore(current);
    ScoreType bestScore = initialScore;
    ScoreManager<ScoreType> scoreManager(initialScore, MAXIMIZE);
    TimeManager timeManager(TIME_LIMIT);

    PRNG prng(SEED);

    do {
        // Draw some number of random moves and apply the best one

        vector<ScoreType> delta(CANDIDATE_MOVES, MAXIMIZE ? -oo : +oo);
        vector<MoveType> candidate(CANDIDATE_MOVES);

        #pragma omp parallel for
        for(size_t r=0; r<CANDIDATE_MOVES; ++r) {
            MoveType mv = DrawRandomMove(prng);
            delta[r] = DeltaCost(current, mv);
            candidate[r] = mv;
        }

        ScoreType bestDelta = MAXIMIZE ? -oo : +oo;
        MoveType bestMove; // SET CORRECT DUMMY MOVE
        bool apply_move = false;
        for(size_t r=0; r<CANDIDATE_MOVES; ++r) {
            if( IsBetterMove(delta[r], bestDelta) ) {
                bestDelta = delta[r];
                bestMove = candidate[r];
                apply_move = true;
            }
        }

        if(apply_move) {
            ApplyMove(current, bestMove);
            scoreManager += bestDelta;
        }

        if(bestDelta * (MAXIMIZE ? +1 : -1) > 0) {
            bestScore = scoreManager.score;
	        best = current;
        }

        timeManager.update();

    }while( timeManager.alive() );

    cerr << "Optimized score: " << bestScore << "\t\t[Improvement: " << 1. * abs(bestScore - initialScore) / bestScore << "]" << endl;
}


ScoreType GetScore(const SolutionType & sol)
{
    ScoreType score=0;

    return score;
}

MoveType DrawRandomMove(PRNG & prng)
{
    MoveType mv; // Assign random move!
    return mv;
}

ScoreType DeltaCost(const SolutionType & sol, const MoveType & mv)
{
    ScoreType delta=0;
    SolutionType tmp(sol);
    ApplyMove(tmp, mv);
    delta = GetScore(tmp) - GetScore(sol);

    #ifndef NDEBUG
        // This will check if your non-trivial implementation of DeltaCost is consistent.
        // This part of the code will be executed only in the debug exe
        ScoreType realDelta = 0;

        SolutionType tmp(sol);
        ApplyMove(tmp, mv);
        realDelta = GetScore(tmp) - GetScore(sol);

        if(realDelta != delta) {
            cerr << "Error: mismatch of Move Cost! Either DeltaCost is wrong or you are not maintaining correctly the state of the solution!" << endl;
            exit(1);
        }
    #endif

    return delta;
}

bool IsBetterMove(ScoreType newDelta, ScoreType oldDelta)
{
    return newDelta * (MAXIMIZE ? +1 : -1) > oldDelta * (MAXIMIZE ? +1 : -1);
}

void ApplyMove(SolutionType & sol, const MoveType & mv)
{

}


void PrintSolution(string filename)
{
    ofstream out(filename);
    
    cerr << "Writing solution to file " << filename << " ... " << endl;

    // Print "best"
}


