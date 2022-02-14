#include <bits/stdc++.h>
#include "utils.cpp"
using namespace std;
array<string, 5> ValidNames = {{"a", "b", "c", "d", "e"}};
string ProblemName = "UNKNOWN";
uint64_t SEED = 0;


void ArgSanitize(int, char**);
void ReadInput(string);
void Greedy();
void PrintSolution(string);




using SolutionType = vector<int>;


int C, K;
map<string, int> ingr_to_id;
vector<string> id_to_ingr;

struct Client {
    int L, D;
    vector<int> likes, dislikes;

    void read(ifstream & cin) {

        cin >> L;
        for(int i=0; i<L; ++i) {
            static string ingr_name;
            cin >> ingr_name;
            if(ingr_to_id.find(ingr_name) == ingr_to_id.end()) {
                cerr << "New ingredient " << ingr_name << "!\n";
                ingr_to_id[ingr_name] = id_to_ingr.size();
                id_to_ingr.emplace_back(ingr_name);
            }

            likes.emplace_back(ingr_to_id[ingr_name]);
        }

        cin >> D;
        for(int i=0; i<D; ++i) {
            static string ingr_name;
            cin >> ingr_name;
            if(ingr_to_id.find(ingr_name) == ingr_to_id.end()) {
                cerr << "New ingredient " << ingr_name << "!\n";
                ingr_to_id[ingr_name] = id_to_ingr.size();
                id_to_ingr.emplace_back(ingr_name);
            }

            dislikes.emplace_back(ingr_to_id[ingr_name]);
        }
    }
};

vector<Client> clients;

SolutionType best;



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

    in >> C;
    clients.resize(C);
    for(auto & client : clients) {
        client.read(in);
    }

    K = ingr_to_id.size();

}

void Greedy()
{
    cerr << "Pot of Greed: draw 2 cards." << endl;

    best.resize(K, 0);
    PRNG rand(SEED);

    for(auto & ingr : best) {
        ingr = rand() % 2;
    }
}

void PrintSolution(string filename)
{
    ofstream out(filename);
    
    cerr << "Writing solution to file " << filename << " ... " << endl;

    int used = accumulate(best.begin(), best.end(), 0);
    out << used << " ";
    for(int i=0; i<K; ++i) {
        if(best[i]) {
            out << id_to_ingr[i] << " ";
        }
    }
    out << endl;
}


