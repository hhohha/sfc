// module: som.h - header file for self organizing map library class
// author: Jan Hammer, xhamme00

#include <vector>

using namespace std;

typedef vector<vector<int>> intMatrix;

// neuron class 
class cNeuron {
public:
    // vector of weights
    vector<int> w;

    // dimension of the neuron
    int size;
    
    // vector for recognizing digits
    vector<int> hits{0,0,0,0,0,0,0,0,0,0};
    
    // constructor
    cNeuron(int size);
    
    // init weigh vector with random values
    void initRandom(int maxn);
    
    // difference between neuron and input vector
    int diffVec(vector<int> v);
    
    // adjust weights by an input vector 
    void addVec(vector<int> v, double coeff);
    
    // get winning digit
    int topHit();
    
    // mark a hit for a certain digit
    void hit(int i);
};

// the net of neurons class
class cSom {
public:
    // vector of neurons (objects of class cNeuron)
    vector<cNeuron> som;
    
    // size of the net and dimensions of the neurons
    unsigned sx, sy, dimension;
    
    // learining coefficient
    double const learnCoeff = 0.1;
    
    // learining coefficient for neighbors
    double const nbrCoeff = 0.75;
    
    // constructor
    cSom(unsigned sx, unsigned sy, unsigned dimension);
    
    // init neuron weights in a grid (only for 2d neurons)
    void initGrid(int mult = 100);
    
    // init neuron weights randomly
    void initRandom(unsigned maxn);
    
    // get neighbors - effectively defines topology of the net
    vector<int> getNeighbors(unsigned n);
    
    // add input vector to winning neuron and neighbors
    void adjustWeights(int idx, vector<int> input);
    
    // go through the vector of inputs, for each find bmu and adjust weights
    void learn(intMatrix input, vector<int> ref, int startIdx = 0, int endIdx = -1);
    
    // go through the vector of inputs, for each find bmu and return the its digit
    void classify(intMatrix input, vector<int> ref, int startIdx = 0, int endIdx = -1);
    
    // print weights of all neurons - used for visualizaton
    void print();
    
    // get the winning neuron
    int getBmu(vector<int> vec);
    
    // getter
    cNeuron getNeuron(unsigned x, unsigned y);
};
