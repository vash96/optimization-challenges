#include <bits/stdc++.h>
using namespace std;

void readInput(string);
void readSolution(string);
void printSolution(string);
int who(int, int);

int getScore(const vector<pair<int, int>>&);
int computeDelta(int, int, const vector<pair<int, int>>&);
void makeMove(const pair<int, int>&, vector<pair<int, int>>&);

void doMagic(int);

struct ScoreManager {
    int score;
    int lastUpdate;
    double threshold;

    ScoreManager(int score, double threshold=0.1)
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


int H, W, D, P;
vector<string> board;
vector<string> company;
vector<int> bonus;
vector<unordered_set<int>> skills;
vector<pair<int, int>> devSeat, pmSeat;
vector<pair<int, int>> solution;
map<string, int> skill2id;

random_device rd;
mt19937 rnd(rd());

map<pair<int, int>, int> whoMap;

int main(int argc, char** argv)
{
    ios_base::sync_with_stdio(false);

    if(argc != 3) {
        cerr << "Wrong number of parameters...\n";
        return 1;
    }

    cerr.precision(2);
    cerr << fixed;

    string filename = argv[1];

    cerr << "Optimizing " << filename << ".out...\n";
    cerr << "Reading input...\n";
    readInput(string("input/") + filename + ".txt");

    cerr << "Reading solution...\n";
    readSolution(string("output/") + filename + ".out");

    cerr << "Read solution of score: " << getScore(solution) << endl;


    cerr << "Invoking black magic powers...\n";
    doMagic(atoi(argv[2]));



    cerr << "Writing optimized solution...\n";
    printSolution(string("output/") + filename + ".opt");

    return 0;
}


void readInput(string filename)
{
    ifstream in(filename);

    // Reading board
    in >> W >> H;
    board.resize(H);
    for(int h=0; h<H; ++h) {
        in >> board[h];
        for(int w=0; w<W; ++w) {
            if(board[h][w] == 'M') {
                pmSeat.emplace_back(h, w);
            }
            if(board[h][w] == '_') {
                devSeat.emplace_back(h, w);
            }
        }
    }

    // Reading devs
    in >> D;
    company.resize(D);
    bonus.resize(D);
    skills.resize(D);
    for(int i=0, nextSkillId=0; i<D; ++i) {
        in >> company[i] >> bonus[i];

        int nskills;
        in >> nskills;
        for(int j=0; j<nskills; ++j) {
            static string skill;
            in >> skill;
            if(skill2id.count(skill) == 0) {
                skill2id[skill] = nextSkillId++;
            }

            skills[i].insert(skill2id[skill]);
        }
    }

    // Reading managers
    in >> P;
    company.resize(D+P);
    bonus.resize(D+P);
    skills.resize(D+P);
    for(int i=D; i<D+P; ++i) {
        in >> company[i] >> bonus[i];
    }

    // resizing utilities
    solution.resize(D+P, {-1, -1});
}


void readSolution(string filename)
{
    ifstream in(filename);
    
    for(int i=0; i<D+P; ++i) {
        string w, h;
        in >> w;

        if(w == "X") continue;
        in >> h;

        int hh = atoi(h.c_str());
        int ww = atoi(w.c_str());
        solution[i] = {hh, ww};
        whoMap[{hh, ww}] = i;
    }
}



void printSolution(string filename)
{
    ofstream out(filename);

    for(const auto & [h, w] : solution) {
        if(h == -1) {
            assert(w == -1);
            out << 'X' << endl;
        }else {
            assert(w != -1);
            out << w << " " << h << endl;
        }
    }
}



void doMagic(int MAXTIME)
{
    auto current = solution;
    int currScore = getScore(current);
    int bestScore = currScore;
    int startScore = currScore;

    ScoreManager scoreManager(bestScore);

    cerr << "Starting score: " << bestScore << endl;

    auto t0 = chrono::high_resolution_clock::now();
    auto t1 = t0;
    do {
        vector<array<int, 3>> moves(500, {{-1, -1, -1}});

        #pragma omp parallel for
        for(int i=0; i<500; ++i) {
            int ty = rnd() % 2;
            int r1, r2;
            if(ty == 0) { // Devs
                r1 = rnd() % D;
                r2 = rnd() % D;
            }else {
                r1 = (rnd() % P) + D;
                r2 = (rnd() % P) + D;
            }

            moves[i] = {{computeDelta(r1, r2, current), r1, r2}};
        }

        pair<int, int> mv = {-1, -1};
        int mvScore = 0;
        for(int i=0; i<500; ++i) {
            if(moves[i][0] > mvScore) {
                mvScore = moves[i][0];
                mv.first = moves[i][1];
                mv.second = moves[i][2];
            }
        }

        makeMove(mv, current);
        currScore += mvScore;
        scoreManager += mvScore;

        if(currScore > bestScore) {
            bestScore = currScore;
            solution = current;
        }

        t1 = chrono::high_resolution_clock::now();

    }while(chrono::duration<double>(t1-t0).count() < MAXTIME);

    cerr << "Optimized to score: " << bestScore << " [+" << (bestScore-startScore) << "]" << endl;
}


int who(int h, int w)
{
    if(whoMap.count({h, w}) == 0) {
        return -1;
    }
    return whoMap[{h, w}];
}

int totalPotential(int i, int j)
{
    if(i == -1 or j == -1 or i==j) {
        return 0;
    }

    int score=0;

    // Bonus potential
    if(company[i] == company[j]) {
        score += bonus[i] * bonus[j];
    }

    // Work potential
    int sUnion = 0;
    for(const auto x : skills[i]) {
        sUnion += skills[j].count(x);
    }
    int sDiff = skills[i].size() + skills[j].size() - 2*sUnion;
    score += sUnion * sDiff;

    return score;
};

int computeScore(int p, int h, int w)
{
    if(h == -1 or w == -1) return 0;

    int score = 0;
    int dh[4] = {+1, -1, 0, 0};
    int dw[4] = {0, 0, +1, -1};

    for(int i=0; i<4; ++i) {
        int hh = h+dh[i];
        int ww = w+dw[i];

        if(0<=hh and hh<H and 0<=ww and ww<W and who(hh, ww) != -1) {
            score += totalPotential(p, who(hh, ww));
        }
    }

    return score;
}

int getScore(const vector<pair<int, int>> & sol)
{
    int score = 0;
    for(int i=0; i<D+P; ++i) {
        score += computeScore(i, sol[i].first, sol[i].second);
    }
    return score / 2;
}



bool adjacent(int a, int b, int c, int d)
{
    return a != -1 and b != -1 and c != -1 and d != -1
        and abs(a-c) + abs(b-d) == 1;
}

int computeDelta(int i, int j, const vector<pair<int, int>> & sol)
{
    assert(i != -1 and j != -1);

    auto [a, b] = sol[i];
    auto [c, d] = sol[j];

    if(a == -1 and c == -1)
        return 0;

    if(i == j)
        return 0;

    int delta = 0;
    delta += computeScore(i, c, d) + computeScore(j, a, b);
    delta -= computeScore(i, a, b) + computeScore(j, c, d);

    if(adjacent(a,b,c,d)) {
        delta += totalPotential(i, j);
    }

    return delta;
}



void makeMove(const pair<int, int> & mv, vector<pair<int, int>> & sol)
{
    auto [i, j] = mv;
    if(i == j or i == -1 or j == -1)
        return;


    auto pos_i = sol[i];
    auto pos_j = sol[j];
    
    sol[i] = pos_j;
    sol[j] = pos_i;

    if(pos_i.first != -1) whoMap[pos_i] = j;
    if(pos_j.first != -1) whoMap[pos_j] = i;
}