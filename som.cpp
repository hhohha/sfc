#include "som.h"

/*
 TODO
  - prepare demo scripts
  - tidy up the code, comments
  - write report
 */


#include <iostream>
#include <cmath>
#include <unistd.h>

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
    for (unsigned i = 0; i < v.size(); i++) {
        sum += pow(v[i] - w[i], 2);
    }
    
    return sum;
}

void cNeuron::addVec(vector<int> v, double coeff) {
    if (v.size() != w.size())
        throw "cNeuron > addVec: wrong vector dimension";
    
    for (unsigned i = 0; i < v.size(); i++) {
        w[i] += round((v[i] - w[i]) * coeff);
    }
}

int cNeuron::topHit() {
    int maxIdx = -1;
    int maxHits = 0;
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

ostream &operator<<(ostream &output, const cNeuron& n) {
    unsigned i = 0;
    for (i = 0; i < n.w.size() - 1; i++) 
        output << n.w[i] << '-';
    if (n.w.size() > 0)
        output << n.w[i];
    return output;
}

    
cSom::cSom(unsigned sx, unsigned sy, unsigned dimension) {
    this->sx = sx;
    this->sy = sy;
    this->dimension = dimension;
    
    for (unsigned i = 0; i < sx * sy; i++){
        som.push_back(cNeuron(dimension));
    } 
}

void cSom::debugPrint() {
    for (unsigned i = 0; i < sx; i++) {
        for (unsigned j = 0; j < sy; j++) {
            cout << som[sx * i + j] << "   ";
        }
        cout << endl;
    }
}

void cSom::initGrid(int mult) {
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
    if (n >= sx)
        retVec.push_back(n - sx);
    if (n < sx * sy - sx)
        retVec.push_back(n + sx);
    if ((n + 1) % sx > 0)
        retVec.push_back(n + 1);
    if (n % sx > 0)
        retVec.push_back(n - 1);
        
    return retVec;
}

void cSom::learn(vector<vector<int>> input, vector<int> ref, int sleepTime, int startIdx, int endIdx) {
    cout << "learning started" << endl;
    int cntr = 0;
    endIdx = endIdx > 0 ? endIdx : input.size();
        
    for (int i = startIdx; i < endIdx; i++) {
        if (cntr == 1000) {
            cout << "parsed " << i << " items out of " << endIdx << endl;
            cntr = 0;
        }
        cntr++;
        usleep(sleepTime);
        
        int bmu = getBmu(input[i]);
        
        som[bmu].addVec(input[i], learnCoeff);
        som[bmu].hit(ref[i]);
        for(int j: getNeighbors(bmu)) {
            som[j].addVec(input[i], learnCoeff * nbrCoeff);
        }
    }
    cout << "learning finished" << endl;
}

void cSom::demo_learn(vector<vector<int>> input, int sleepTime, int startIdx, int endIdx) {
    endIdx = endIdx > 0 ? endIdx : input.size();
    print({-1, -1});
    
    for (int i = startIdx; i < endIdx; i++) {
        usleep(sleepTime);
        
        int bmu = getBmu(input[i]);
        
        som[bmu].addVec(input[i], learnCoeff);
        for(int j: getNeighbors(bmu)) {
            som[j].addVec(input[i], learnCoeff * nbrCoeff);
        }
        
        print(input[i]);
    }
    cout << "0x0";
}

void cSom::classify(vector<vector<int>> input, vector<int> ref, int startIdx, int endIdx) {
    cout << "classification started" << endl;
    int correct = 0, wrong = 0;
    
    endIdx = endIdx > 0 ? endIdx : input.size();
    
    for (int i = startIdx; i < endIdx; i++) {
        int bmu = getBmu(input[i]);
        if (som[bmu].topHit() == ref[i]) {
            correct++;
        } else {
            wrong++;
        }
    }
    
    cout << "classification ended" << endl;
    cout << "correctly classified: " << correct << " items out of " << correct + wrong << endl;
    cout << "wrongly classified: " << wrong << " items out of " << correct + wrong << endl;
}
    
void cSom::print(vector<int> input) {
    cout << sx << "x" << sy << endl;
    cout << input[0] << " " << input[1] << endl;
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
