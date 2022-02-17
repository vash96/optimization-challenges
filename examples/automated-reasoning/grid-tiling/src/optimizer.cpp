#include <bits/stdc++.h>
#include "utils.cpp"    // ScoreManager, TimeManager, Random

#define eee(x, y) (board[x][y] == Tile::EEE)

using namespace std;
using namespace chrono;

constexpr array<const char*, 1> ValidNames = {{"1"}};
string ProblemName;
uint64_t SEED = 0;
size_t CANDIDATE_MOVES = 1;
seconds TIME_LIMIT(oo);

bool MAXIMIZE = true;

double GlobalTemperature = 1.0, CoolingRate = 0.9999;
random_device rd;
default_random_engine eng(rd());
uniform_real_distribution<double> prob(0.0, 1.0);


enum Tile { 
    XXX, 
    EEE, 
    
    S11, S12, 
    S13, S14, 
    
    R11, R12, R13,
    
    R21, 
    R22, 
    R23, 
                            
    L11, 
    L12, L13,
    
    L22, L21, 
    L23, 
    
    L33, L32, 
         L31, 
    
         L43, 
    L41, L42
};

array<string, 24> sym = {{ 
    "XXX", 
    "EEE", 
    
    "S11", "S12", 
    "S13", "S14", 
    
    "R11", "R12", "R13",
    
    "R21", 
    "R22", 
    "R23", 
                            
    "L11", 
    "L12", "L13",
    
    "L22", "L21", 
    "L23", 
    
    "L33", "L32", 
           "L31", 
    
           "L43", 
    "L41", "L42" 
}};

map<string, Tile> antisym = {
    {"XXX", Tile::XXX},
    {"EEE", Tile::EEE},

    {"S11", Tile::S11}, {"S12", Tile::S12}, 
    {"S13", Tile::S13}, {"S14", Tile::S14}, 

    {"R11", Tile::R11}, {"R12", Tile::R12}, {"R13", Tile::R13},

    {"R21", Tile::R21},
    {"R22", Tile::R22},
    {"R23", Tile::R23},

    {"L11", Tile::L11}, 
    {"L12", Tile::L12}, {"L13", Tile::L13},
    
    {"L22", Tile::L22}, {"L21", Tile::L21},
    {"L23", Tile::L23}, 
    
    {"L33", Tile::L33}, {"L32", Tile::L32}, 
                        {"L31", Tile::L31}, 
    
                        {"L43", Tile::L43}, 
    {"L41", Tile::L41}, {"L42", Tile::L42}
};


using ScoreType = int; // CHANGE IF NEEDED
struct SolutionType {
    int s, r, l;
    vector<vector<Tile>> board;

    vector<Tile> & operator[](int i) {
        return board[i];
    }

    const vector<Tile> & operator[](int i) const {
        return board[i];
    }
};

struct MoveType {
    int x, y;
    Tile tile;
};

array<Tile, 8> drawable = {{EEE, S11, R11, R21, L11, L21, L31, L41}};

int n, s, r, l, f;
vector<pair<int, int>> forbidden;

int S1(const SolutionType&, int, int);

int R1(const SolutionType&, int, int);
int R2(const SolutionType&, int, int);

int L1(const SolutionType&, int, int);
int L2(const SolutionType&, int, int);
int L3(const SolutionType&, int, int);
int L4(const SolutionType&, int, int);

array< function<int(const SolutionType&,int,int)>, 7 > costFns = {{S1, R1, R2, L1, L2, L3, L4}};
map<Tile, int> posFns = {
    {Tile::S11, 0},
    {Tile::R11, 1},
    {Tile::R21, 2},
    {Tile::L11, 3},
    {Tile::L21, 4},
    {Tile::L31, 5},
    {Tile::L41, 6},
};

void Insert(SolutionType&, const MoveType&);


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

bool Feasible(const SolutionType&, const MoveType&);




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

    in >> n >> s >> r >> l >> f;
    for(int i=0; i<f; ++i) {
        int x, y;
        in >> x >> y;
        forbidden.emplace_back(x+1, y+1);
    }
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

    initial.s = s;
    initial.r = r;
    initial.l = l;
    initial.board.resize(n+4, vector<Tile>(n+4, Tile::EEE));
    for(int i=0; i<=n+3; ++i) {
        initial[0][i] = initial[i][0] = initial[n+3][i] = initial[i][n+3] =
        initial[1][i] = initial[i][1] = initial[n+2][i] = initial[i][n+2] = Tile::XXX;
    }
    for(int x=2; x<=n+1; ++x) {
        for(int y=2; y<=n+1; ++y) {
            static string cell;
            in >> cell;
            initial[x][y] = antisym[cell];
            switch(initial[x][y]) {
                case Tile::S11:
                    --initial.s;
                    break;
                
                case Tile::R11:
                case Tile::R21:
                    --initial.r;
                    break;
                
                case Tile::L11:
                case Tile::L21:
                case Tile::L31:
                case Tile::L41:
                    --initial.l;
                    break;
            }
        }
    }
}

void DoMagic()
{
    cerr << "Invoking the power of the underworld..." << endl;

    SolutionType current = best = initial;
    ScoreType initialScore = GetScore(current);
    ScoreType bestScore = initialScore;
    ScoreManager<ScoreType> scoreManager(initialScore, MAXIMIZE);
    TimeManager timeManager(TIME_LIMIT);
    TimeManager<false> coolingSchedule(TIME_LIMIT, 5ms);

    int64_t n_iter = 0;

    PRNG prng(SEED);

    cerr << "Let's start the magic!\n";
    cerr << "Initial score: " << initialScore << "\n";
    do {
        // Draw some number of random moves and apply the best one
        ++n_iter;

        vector<ScoreType> delta(CANDIDATE_MOVES, MAXIMIZE ? -oo : +oo);
        vector<MoveType> candidate(CANDIDATE_MOVES);

        #pragma omp parallel for if(CANDIDATE_MOVES >= 8000)
        for(size_t r=0; r<CANDIDATE_MOVES; ++r) {
            MoveType mv = DrawRandomMove(prng);
            delta[r] = DeltaCost(current, mv);
            candidate[r] = mv;
        }

        ScoreType bestDelta = MAXIMIZE ? -oo : +oo;
        MoveType bestMove = {-1, -1, Tile::EEE}; // SET CORRECT DUMMY MOVE
        bool apply_move = false;
        for(size_t r=0; r<CANDIDATE_MOVES; ++r) {
            if( Feasible(current, candidate[r]) and IsBetterMove(delta[r], bestDelta) ) {
                bestDelta = delta[r];
                bestMove = candidate[r];
                apply_move = true;
            }
        }
        
        if(apply_move) {
            ApplyMove(current, bestMove);
            scoreManager += bestDelta;
        }

        if(scoreManager.score * (MAXIMIZE ? +1 : -1) > bestScore * (MAXIMIZE ? +1 : -1)) {
            bestScore = scoreManager.score;
	        best = current;
        }

        timeManager.update();
        coolingSchedule.update();

        if(coolingSchedule.event()) {
            GlobalTemperature *= CoolingRate;
        }

    }while( timeManager.alive() );

    cerr << "Optimized score: " << bestScore << "\t\t[Improvement: " << 1. * abs(bestScore - initialScore) / bestScore << "]" << endl;
    cerr << "Number of iterations: " << n_iter/1'000'000 << " millions\n";
}


ScoreType GetScore(const SolutionType & sol)
{
    ScoreType score=0;

    for(const auto & row : sol.board) {
        for(const auto val : row) {
            score += val == Tile::EEE;
        }
    }

    return score;
}

MoveType DrawRandomMove(PRNG & prng)
{
    MoveType mv = {prng() % uint64_t(n) +2, prng() % uint64_t(n) +2, drawable[ prng() % 8 ]}; // Assign random move!
    return mv;
}

bool Feasible(const SolutionType & sol, const MoveType & mv)
{
    if(mv.tile == Tile::EEE) {
        int x = mv.x, y = mv.y;
        return  sol[x][y] == Tile::S11 or sol[x][y] == Tile::R11 or sol[x][y] == Tile::R21 or
                sol[x][y] == Tile::L11 or sol[x][y] == Tile::L21 or sol[x][y] == Tile::L31 or sol[x][y] == Tile::L41;
    }

    if( (mv.tile == Tile::S11 and sol.s == 0) or
        ((mv.tile == Tile::R11 or mv.tile == Tile::R21) and sol.r == 0) or
        ((mv.tile == Tile::L11 or mv.tile == Tile::L21 or mv.tile == Tile::L31 or mv.tile == Tile::L41) and sol.l == 0)) {
            return false;
    }

    int p = posFns[mv.tile];
    int cost = costFns[p](sol, mv.x, mv.y);

    return cost == 0;
}

ScoreType DeltaCost(const SolutionType & sol, const MoveType & mv)
{
    if(not Feasible(sol, mv)) {
        return oo;
    }

    ScoreType delta=0;
    SolutionType tmp(sol);
    ApplyMove(tmp, mv);
    delta = GetScore(tmp) - GetScore(sol);

    #ifndef NDEBUG
        // This will check if your non-trivial implementation of DeltaCost is consistent.
        // This part of the code will be executed only in the debug exe
        ScoreType realDelta = 0;

        SolutionType tmptmp(sol);
        ApplyMove(tmptmp, mv);
        realDelta = GetScore(tmptmp) - GetScore(sol);

        if(realDelta != delta) {
            cerr << "Error: mismatch of Move Cost! Either DeltaCost is wrong or you are not maintaining correctly the state of the solution!" << endl;
            exit(1);
        }
    #endif

    return delta;
}

bool IsBetterMove(ScoreType newDelta, ScoreType oldDelta)
{
    if( newDelta * (MAXIMIZE ? -1 : +1) > 0.0 ) {
        double p = exp((MAXIMIZE ? +1 : -1) * newDelta / GlobalTemperature);
        return p > prob(eng);
    }

    return true;
}

void ApplyMove(SolutionType & sol, const MoveType & mv)
{
    Insert(sol, mv);
}


void PrintSolution(string filename)
{
    ofstream out(filename);
    
    cerr << "Writing solution to file " << filename << " ... " << endl;

    // Print "best"
    for(int x=2; x<=n+1; ++x) {
        for(int y=2; y<=n+1; ++y) {
            out << sym[ best[x][y] ] << " ";
        }
        out << endl;
    }

    out << "\n\nEmpty = " << GetScore(best) << "\n\n";
}




int S1(const SolutionType & board, int x, int y) {
    return 4 - (eee(x,y) + eee(x,y+1) + eee(x+1,y) + eee(x+1,y+1));
}

int R1(const SolutionType & board, int x, int y) {
    return 3 - (eee(x,y) + eee(x,y+1) + eee(x,y+2));
}

int R2(const SolutionType & board, int x, int y) {
    return 3 - (eee(x,y) + eee(x+1,y) + eee(x+2,y));
}


int L1(const SolutionType & board, int x, int y) {
    return 3 - (eee(x,y) + eee(x+1,y) + eee(x+1,y+1));
}

int L2(const SolutionType & board, int x, int y) {
    return 3 - (eee(x,y-1) + eee(x,y) + eee(x+1,y-1));
}

int L3(const SolutionType & board, int x, int y) {
    return 3 - (eee(x-1,y) + eee(x-1,y-1) + eee(x,y));
}

int L4(const SolutionType & board, int x, int y) {
    return 3 - (eee(x,y) + eee(x,y+1) + eee(x-1,y+1));
}



void Insert(SolutionType & board, const MoveType & mv) {
    int x = mv.x, y = mv.y;
    Tile tile = mv.tile;
    int & s = board.s;
    int & r = board.r;
    int & l = board.l;

    switch(tile) {
        case Tile::EEE:
            switch(board[x][y]) {
                case Tile::S11:
                    ++s;
                    board[x][y] = board[x][y+1] = board[x+1][y] = board[x+1][y+1] = Tile::EEE;
                    break;
                case Tile::R11:
                    ++r;
                    board[x][y] = board[x][y+1] = board[x][y+2] = Tile::EEE;
                    break;
                case Tile::R21:
                    ++r;
                    board[x][y] = board[x+1][y] = board[x+2][y] = Tile::EEE;
                    break;
                case Tile::L11:
                    ++l;
                    board[x][y] = board[x+1][y] = board[x+1][y+1] = Tile::EEE;
                    break;
                case Tile::L21:
                    ++l;
                    board[x][y] = board[x][y-1] = board[x+1][y-1] = Tile::EEE;
                    break;
                case Tile::L31:
                    ++l;
                    board[x][y] = board[x-1][y] = board[x-1][y-1] = Tile::EEE;
                    break;
                case Tile::L41:
                    ++l;
                    board[x][y] = board[x][y+1] = board[x-1][y+1] = Tile::EEE;
                    break;

                default:
                    cerr << "Unknown case in inner switch!\n";
                    cerr << "Tile is " << sym[board[x][y]] << "\n";
                    exit(1);
            }
            break;
        case Tile::S11:
            --s;
            board[x][y] = Tile::S11;    board[x][y+1] = Tile::S12;
            board[x+1][y] = Tile::S13;  board[x+1][y+1] = Tile::S14;
            break;
        case Tile::R11:
            --r;
            board[x][y] = Tile::R11; board[x][y+1] = Tile::R12; board[x][y+2] = Tile::R13;
            break;
        case Tile::R21:
            --r;
            board[x][y] = Tile::R21; 
            board[x+1][y] = Tile::R22; 
            board[x+2][y] = Tile::R23;
            break;
        case Tile::L11:
            --l;
            board[x][y] = Tile::L11;
            board[x+1][y] = Tile::L12;  board[x+1][y+1] = Tile::L13;
            break;
        case Tile::L21:
            --l;
            board[x][y-1] = Tile::L22;    board[x][y] = Tile::L21;
            board[x+1][y-1] = Tile::L23;
            break;
        case Tile::L31:
            --l;
            board[x-1][y-1] = Tile::L33;    board[x-1][y] = Tile::L32;
                                            board[x][y] = Tile::L31;
            break;
        case Tile::L41:
            --l;
                                        board[x-1][y+1] = Tile::L43;
            board[x][y] = Tile::L41;    board[x][y+1] = Tile::L42;
            break;
        default:
            cerr << "Unknown case in outer switch!\n";
            cerr << "Tile is " << sym[tile] << "\n";
            exit(1);
    }
}