// module: som.h - file for self organizing map library class
// author: Jan Hammer, xhamme00

#include <iostream>
#include <cmath>
#include <unistd.h>
#include "som.h"

using namespace std;
    
cNeuron::cNeuron(int size) {
    w.resize(size);
    this->size = size;
}

void cNeuron::initRandom(int maxn) {
    for (unsigned i = 0; i < w.size(); i++) 
        w[i] = rand() % maxn;
}

int cNeuron::diffVec(vector<int> v) {
    if (v.size() != w.size())
        throw "cNeuron > diffVec: wrong vector dimension";
    
    int sum = 0;
    
    // difference between neuron and vector is done item by item then squared and summed together
    for (unsigned i = 0; i < v.size(); i++) {
        sum += pow(v[i] - w[i], 2);
    }
    
    return sum;
}

void cNeuron::addVec(vector<int> v, double coeff) {
    if (v.size() != w.size())
        throw "cNeuron > addVec: wrong vector dimension";
    
    // adjust vector - item by item the difference is multiplied by a learning coefficient, then added
    for (unsigned i = 0; i < v.size(); i++) {
        w[i] += round((v[i] - w[i]) * coeff);
    }
}

int cNeuron::topHit() {
    int maxIdx = -1;
    int maxHits = 0;
    
    // get the maximum index from hits vector - that is the winning digit
    for (unsigned i = 0; i < hits.size(); i++) {
        if (hits[i] > maxHits) {
            maxHits = hits[i];
            maxIdx = i;
        }
    }
    
    return maxIdx;
}

void cNeuron::hit(int i) {
    hits[i]++;
}


cSom::cSom(unsigned sx, unsigned sy, unsigned dimension) {
    this->sx = sx;
    this->sy = sy;
    this->dimension = dimension;
    
    for (unsigned i = 0; i < sx * sy; i++){
        som.push_back(cNeuron(dimension));
    } 
}

void cSom::initGrid(int mult) {
    if (dimension != 2)
        throw "initGrid is possible only for 2D SOM";
    
    for (unsigned i = 0; i < sx; i++)
        for (unsigned j = 0; j < sy; j++) {
            som[i * sx + j].w[0] = i * mult;
            som[i * sx + j].w[1] = j * mult;
        }
}

void cSom::initRandom(unsigned maxn) {
    for (unsigned i = 0; i < sx; i++)
        for (unsigned j = 0; j < sy; j++)
            som[i * sx + j].initRandom(maxn);
}

vector<int> cSom::getNeighbors(unsigned n) {
    vector<int> retVec;
    
    // get the neighbor (if it is there)
    if (n >= sx)  // look up
        retVec.push_back(n - sx);
    if (n < sx * sy - sx)  // look down
        retVec.push_back(n + sx);
    if ((n + 1) % sx > 0)  // look right
        retVec.push_back(n + 1); 
    if (n % sx > 0) // look left
        retVec.push_back(n - 1);
        
    return retVec;
}

void cSom::adjustWeights(int idx, vector<int> v) {
    // winning neuron multiplies the adjustment by learning coefficient
    som[idx].addVec(v, learnCoeff);
    
    //neighboring neurons also by neighboring coefficient
    for(int i: getNeighbors(idx)) {
        som[i].addVec(v, learnCoeff * nbrCoeff);
    }
}

void cSom::learn(intMatrix data, vector<int> ref, int startIdx, int endIdx) {
    cout << "PROGRESS: learning started" << endl;
    int cntr = 0;
    endIdx = endIdx > 0 ? endIdx : data.size();
        
    // get bmu -> adjust weights -> repeat
    for (int i = startIdx; i < endIdx; i++) {
        
        // primitive progress info
        if (cntr == 1000) {
            cout << "PROGRESS: learning: " << i << " items out of " << endIdx << endl;
            cntr = 0;
        }
        cntr++;
        
        int bmu = getBmu(data[i]);
        
        som[bmu].hit(ref[i]);
        adjustWeights(bmu, data[i]);
    }
    cout << "PROGRESS: learning finished" << endl;
}

void cSom::classify(intMatrix data, vector<int> ref, int startIdx, int endIdx) {
    cout << "PROGRESS: classification started" << endl;
    int correct = 0, wrong = 0;
    
    endIdx = endIdx > 0 ? endIdx : data.size();
    
    // get bmu -> compare with reference -> repeat
    for (int i = startIdx; i < endIdx; i++) {
        int bmu = getBmu(data[i]);
        if (som[bmu].topHit() == ref[i]) {
            correct++;
        } else {
            wrong++;
        }
    }
    
    cout << "PROGRESS: classification ended" << endl;
    
    cout << "\ncorrectly classified: " << correct << " items out of " << correct + wrong;
    cout << " (" << correct * 100 / double(correct + wrong) << "%)" << endl;
    cout << "wrongly classified: " << wrong << " items out of " << correct + wrong;
    cout << " (" << wrong * 100 / double (correct + wrong) << "%)" << endl;
    
}
    
void cSom::print() {
    for (unsigned i = 0; i < som.size(); i++) {
        for (auto n: som[i].w) {
            cout << n << " ";
        }
        cout << endl;
    }
}

int cSom::getBmu(vector<int> vec) {
    int minval = 0;
    int bestNeuronIdx = -1;
    
    for (unsigned i = 0; i < som.size(); i++) {
        int tmp = som[i].diffVec(vec);
        if (tmp < minval || bestNeuronIdx == -1) {
            minval = tmp;
            bestNeuronIdx = i;
        }
    }
    
    return bestNeuronIdx;
}

cNeuron cSom::getNeuron(unsigned x, unsigned y) {
    unsigned i = sx * x + y;
    
    if (i >= som.size())
        throw "cSom > getNeuron: attemted to get neuron with bad index";
    return som[i];
}
