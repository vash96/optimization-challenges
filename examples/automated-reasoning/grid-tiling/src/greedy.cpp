#include <bits/stdc++.h>
#include "utils.cpp"

#define eee(x, y) (board[x][y] == Tile::EEE)

using namespace std;
array<string, 1> ValidNames = {{"1"}};
string ProblemName = "UNKNOWN";
uint64_t SEED = 0;


void ArgSanitize(int, char**);
void ReadInput(string);
void Greedy();
void PrintSolution(string);

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

vector<vector<Tile>> board;
array<Tile, 9> drawable = {{XXX, EEE, S11, R11, R21, L11, L21, L31, L41}};

int n, s, r, l, f;



int S1(int, int);

int R1(int, int);
int R2(int, int);

int L1(int, int);
int L2(int, int);
int L3(int, int);
int L4(int, int);


void Insert(Tile, int, int);




int main(int argc, char** argv)
{
    ios_base::sync_with_stdio(false);
    ArgSanitize(argc, argv);

    ReadInput( string("input/") + ProblemName + ".txt" );
    Greedy();
    PrintSolution( string("output/") + ProblemName + ".greedy" );

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
            }else {
                cerr << "Unknown argument --" << arg << ". Valid arguments are: " << endl;
                cerr << "\t--problem <problem name>" << endl;
                cerr << "\t--seed <seed value>" << endl;
                exit(1);
            }
        }else {
            cerr << "Wrong form of argument. Must be pre-pended with `--`." << endl;
            exit(1);
        }
    }

    if(ProblemName == "UNKNOWN") {
        cerr << "Missing problem name. Use `--problem <problem name>` argument!" << endl;
    }

    if(SEED == 0) {
        cerr << "Warning! SEED set to 0!" << endl;
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
    board.resize(n+4, vector<Tile>(n+4, Tile::EEE));
    for(int i=0; i<=n+3; ++i) {
        board[0][i] = board[i][0] = board[n+3][i] = board[i][n+3] = Tile::XXX;
        board[1][i] = board[i][1] = board[n+2][i] = board[i][n+2] = Tile::XXX;
    }

    for(int i=0; i<f; ++i) {
        int x, y;
        in >> x >> y;
        board[x+1][y+1] = Tile::XXX;
    }
}

void Greedy()
{
    cerr << "Pot of Greed: draw 2 cards." << endl;

    for(int x=2; x<=n+1; ++x) {
        for(int y=2; y<=n+1; ++y) {
            int cost[9] = {
                1,
                1,
                (s == 0) + S1(x, y),
                (r == 0) + R1(x, y),
                (r == 0) + R2(x, y),
                (l == 0) + L1(x, y),
                (l == 0) + L2(x, y),
                (l == 0) + L3(x, y),
                (l == 0) + L4(x, y)
            };
            
            int p=2;
            for(; p<9 and cost[p] > 0; ++p) { }

            if(p < 9) {
                Insert(drawable[p], x, y);
            }
        }

    }
}

void PrintSolution(string filename)
{
    ofstream out(filename);
    
    cerr << "Writing solution to file " << filename << " ... " << endl;
    int cost = 0;
    for(int x=2; x<=n+1; ++x) {
        for(int y=2; y<=n+1; ++y) {
            out << sym[board[x][y]] << " ";
            cost += board[x][y] == Tile::EEE;
        }
        out << endl;
    }

    out << "\n\nEmpty = " << cost << "\n\n";
}


int S1(int x, int y) {
    return 4 - (eee(x,y) + eee(x,y+1) + eee(x+1,y) + eee(x+1,y+1));
}

int R1(int x, int y) {
    return 3 - (eee(x,y) + eee(x,y+1) + eee(x,y+2));
}

int R2(int x, int y) {
    return 3 - (eee(x,y) + eee(x+1,y) + eee(x+2,y));
}


int L1(int x, int y) {
    return 3 - (eee(x,y) + eee(x+1,y) + eee(x+1,y+1));
}

int L2(int x, int y) {
    return 3 - (eee(x,y-1) + eee(x,y) + eee(x+1,y-1));
}

int L3(int x, int y) {
    return 3 - (eee(x-1,y) + eee(x-1,y-1) + eee(x,y));
}

int L4(int x, int y) {
    return 3 - (eee(x-1,y+1) + eee(x,y+1) + eee(x,y));
}


void Insert(Tile tile, int x, int y) {
    switch(tile) {
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
            assert(board[x][y] == Tile::EEE and board[x+1][y] == Tile::EEE and  board[x+1][y+1] == Tile::EEE);
            board[x][y] = Tile::L11;
            board[x+1][y] = Tile::L12;  board[x+1][y+1] = Tile::L13;
            break;
        case Tile::L21:
            --l;
            assert(board[x][y] == Tile::EEE and board[x+1][y-1] == Tile::EEE and  board[x+1][y-1] == Tile::EEE);
            board[x][y-1] = Tile::L22;    board[x][y] = Tile::L21;
            board[x+1][y-1] = Tile::L23;
            break;
        case Tile::L31:
            --l;
            assert(board[x][y] == Tile::EEE and board[x-1][y-1] == Tile::EEE and  board[x-1][y] == Tile::EEE);
            board[x-1][y-1] = Tile::L33;    board[x-1][y] = Tile::L32;
                                            board[x][y] = Tile::L31;
            break;
        case Tile::L41:
            --l;
            assert(board[x][y] == Tile::EEE and board[x-1][y+1] == Tile::EEE and  board[x][y+1] == Tile::EEE);
                                        board[x-1][y+1] = Tile::L43;
            board[x][y] = Tile::L41;    board[x][y+1] = Tile::L42;
            break;
        default:
            cerr << "Dio bono svegliati!\n";
            exit(1);
    }
}