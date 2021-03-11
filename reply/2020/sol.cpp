#include <bits/stdc++.h>
using namespace std;

void readInput();
void printSolution();
void greedy();
int computeScore(int,int,int);
void assign(int,int,int);
int totalPotential(int, int);
int who(int, int);


int H, W, D, P;
vector<string> board;
vector<string> company;
vector<int> bonus;
vector<unordered_set<int>> skills;
vector<pair<int, int>> devSeat, pmSeat;
vector<pair<int, int>> solution;
map<string, int> skill2id;

map<pair<int, int>, int> whoMap;

int main(int argc, char** argv)
{
    ios_base::sync_with_stdio(false);

    if(argc != 2) {
        cerr << "Wrong number of parameters...\n";
        return 1;
    }

    cerr.precision(2);
    cerr << fixed;

    string filename = argv[1];

    cerr << "Solving " << filename << ".txt...\n";
    freopen((string("input/") + filename + ".txt").c_str(), "r", stdin);
    freopen((string("output/") + filename + ".out").c_str(), "w", stdout);

    cerr << "Reading input...\n";
    readInput();

    cerr << "Computing initial solution...\n";
    greedy();



    cerr << "Printing solution...\n";
    printSolution();

    return 0;
}



void readInput()
{
    // Reading board
    cin >> W >> H;
    board.resize(H);
    for(int h=0; h<H; ++h) {
        cin >> board[h];
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
    cin >> D;
    company.resize(D);
    bonus.resize(D);
    skills.resize(D);
    for(int i=0, nextSkillId=0; i<D; ++i) {
        cin >> company[i] >> bonus[i];

        int nskills;
        cin >> nskills;
        for(int j=0; j<nskills; ++j) {
            static string skill;
            cin >> skill;
            if(skill2id.count(skill) == 0) {
                skill2id[skill] = nextSkillId++;
            }

            skills[i].insert(skill2id[skill]);
        }
    }

    // Reading managers
    cin >> P;
    company.resize(D+P);
    bonus.resize(D+P);
    skills.resize(D+P);
    for(int i=D; i<D+P; ++i) {
        cin >> company[i] >> bonus[i];
    }

    // resizing utilities
    solution.resize(D+P, {-1, -1});
}



void printSolution()
{
    for(const auto & [h, w] : solution) {
        if(h == -1) {
            assert(w == -1);
            cout << 'X' << endl;
        }else {
            assert(w != -1);
            cout << w << " " << h << endl;
        }
    }
}





double last = -6.0;

void completion(long long comparisons, long long totalComparisons)
{
    double perc = 100.0 * comparisons / totalComparisons;
    if(perc - last > 5.0) {
        last = perc;
        cerr << "[Percentage of completion: " << perc << "]%\n";
    }
}

void greedy()
{
    const auto findBest = [&](int st, int en, int h, int w) -> pair<int, int> {
        vector<int> scores(en-st, -1);

        #pragma omp parallel for
        for(int i=st; i<en; ++i) {
            if(solution[i].first == -1) {
                scores[i-st] = computeScore(i, h, w);
            }
        }

        int bestReplyer = -1;
        int bestScore = -1;
        for(int i=st; i<en; ++i) {
            if(scores[i-st] > bestScore) {
                bestScore = scores[i-st];
                bestReplyer = i;
            }
        }

        return {bestReplyer, bestScore};
    };


    int score = 0;
    long long totalComparisons = static_cast<long long>(devSeat.size()) * static_cast<long long>(D)
                                + static_cast<long long>(pmSeat.size()) * static_cast<long long>(P);

    long long comparisons = 0;

    completion(comparisons, totalComparisons);
    // Assigning developers to seats
    for(const auto & [h, w] : devSeat) {
        const auto [dev, devScore] = findBest(0, D, h, w);
        if(dev != -1) {
            assign(dev, h, w);
            score += devScore;
        }
        comparisons += D;
        completion(comparisons, totalComparisons);
    }

    // Assigning project managers to seats
    for(const auto & [h, w] : pmSeat) {
        const auto [pm, pmScore] = findBest(D, D+P, h, w);
        if(pm != -1) {
            assign(pm, h, w);
            score += pmScore;
        }
        comparisons += P;
        completion(comparisons, totalComparisons);
    }

    cerr << "Greedy solution score: " << score << endl;
}



int computeScore(int p, int h, int w)
{
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



void assign(int p, int h, int w)
{
    assert(who(h, w) == -1);
    whoMap[{h, w}] = p;
    solution[p] = {h, w};
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
    if(i == -1 or j == -1) {
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
}



