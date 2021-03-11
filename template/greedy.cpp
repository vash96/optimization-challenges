#include <bits/stdc++.h>
using namespace std;
constexpr array<const char*, 6> validNames = {{"a", "b", "c", "d", "e", "f"}};
string problemName;


void argSanitize(int, char**);
void readInput(string);
void greedy();
void printSolution(string);


int main(int argc, char** argv)
{
    ios_base::sync_with_stdio(false);
    argSanitize(argc, argv);

    readInput( string("input/") + problemName + ".txt" );
    greedy();
    printSolution( string("output/") + problemName + ".greedy" );

    return 0;
}


void argSanitize(int argc, char** argv)
{
    if(argc != 2) {
        cerr << "Wrong number of parameters...\n";
        cerr << "Usage: " << argv[0] << " <problem-name>" << endl;
        exit(1);
    }

    problemName = argv[1];

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

void greedy()
{
    cerr << "Pot of Greed: draw 2 cards." << endl;
}

void printSolution(string filename)
{
    ofstream out(filename);
    
    cerr << "Writing solution to file " << filename << " ... " << endl;
}


