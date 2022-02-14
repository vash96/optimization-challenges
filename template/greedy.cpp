#include <bits/stdc++.h>
#include "utils.cpp"
using namespace std;
constexpr array<const char*, 1> ValidNames = {{"ADD-VALID-NAMES!!"}};
string ProblemName;
uint64_t SEED = 0;


void ArgSanitize(int, char**);
void ReadInput(string);
void Greedy();
void PrintSolution(string);


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
            if(arg.c_str() == "problem") {
                if(i+1 < argc) {
                    ProblemName = argv[i+1];
                    i += 2;

                    if(find(ValidNames.begin(), ValidNames.end(), ProblemName.c_str())
                        == ValidNames.end()
                    ) {
                        cerr << "Problem name is not valid!" << endl;
                        exit(1);
                    }
                }else {
                    cerr << "Missing name after --problem argument!" << endl;
                    exit(1);
                }
            }else if(arg.c_str() == "seed") {
                if(i+1 < argc) {
                    SEED = atoi(argv[i+1]);
                    i += 2;
                }else {
                    cerr << "Missing seed after --seed argument!" << endl;
                    exit(1);
                }
            }else {
                cerr << "Unknown argument --" << arg << ". Valid arguments are: " << endl;
                cerr << "\t--problem <problem name>" << endl;
                cerr << "\t--seed <seed value>" << endl;
                exit(1);
            }
        }
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

void Greedy()
{
    cerr << "Pot of Greed: draw 2 cards." << endl;
}

void PrintSolution(string filename)
{
    ofstream out(filename);
    
    cerr << "Writing solution to file " << filename << " ... " << endl;
}


