#include <vector>

using namespace std;

typedef vector<vector<int>> intMatrix;

class cNeuron {
public:
    vector<int> w;
    int size;
    vector<int> hits{0,0,0,0,0,0,0,0,0,0};
    
    cNeuron(int size);
    void initRandom(int maxn);
    int diffVec(vector<int> v);
    void addVec(vector<int> v, double coeff);
    int topHit();
    void hit(int i);
};

class cSom {
public:    
    vector<cNeuron> som;
    unsigned sx, sy, dimension;
    double const learnCoeff = 0.1;
    double const nbrCoeff = 0.75;
    
    cSom(unsigned sx, unsigned sy, unsigned dimension);
    void initGrid(int mult = 100);
    void initRandom(unsigned maxn);
    vector<int> getNeighbors(unsigned n);
    void adjustWeights(int idx, vector<int> input);
    void learn(intMatrix input, vector<int> ref, int startIdx = 0, int endIdx = -1);
    void classify(intMatrix input, vector<int> ref, int startIdx = 0, int endIdx = -1);
    void print();
    int getBmu(vector<int> vec);
    cNeuron getNeuron(unsigned x, unsigned y);
};
