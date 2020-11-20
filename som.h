#include <vector>

using namespace std;

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
    void debugPrint();
    void initGrid(int mult = 100);
    void initRandom(unsigned maxn);
    vector<int> getNeighbors(unsigned n);
    void learn(vector<vector<int>> input, vector<int> ref, int sleepTime = 0, int startIdx = 0, int endIdx = -1);
    void demo_learn(vector<vector<int>> input, int sleepTime = 0, int startIdx = 0, int endIdx = -1);
    void classify(vector<vector<int>> input, vector<int> ref, int startIdx = 0, int endIdx = -1);
    void print(vector<int> input);
    int getBmu(vector<int> vec);
    cNeuron getNeuron(unsigned x, unsigned y);
};